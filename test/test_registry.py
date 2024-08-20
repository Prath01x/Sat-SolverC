import os
import re

solver_bin = "bin/satsolver_opt"
test_bin = "bin/testrunner"

tests = {
    'public.stack.empty',
    'public.stack.emptyclear',
    'public.stack.singlepush',
    'public.stack.singlepop',
    'public.stack.singlepeek',

    'public.parser.simple01_valid',
    'public.parser.simple02_valid',
    'public.parser.simple03_invalid',
    'public.parser.simple04_invalid',

    'public.cnf.variable',
    'public.cnf.tseitin01',

    'public.solver.simple01_sat',
    'public.solver.complex00_sat',
    'public.solver.complex00_unsat',
    'public.solver.valid_output_sat',
    'public.solver.minisudoku01_sat',
}

def validate_mapping(map_str, formula_path):
    exp_str = '  ([a-zA-Z0-9]+) -> (TRUE|FALSE)'
    exp = re.compile(exp_str)

    mapping = {}

    for line in map_str.splitlines():
        m = exp.match(line)
        if m:
            name = m.group(1)
            val_str = m.group(2)
            if val_str == 'TRUE':
                val = True
            else:
                val = False
            if name in mapping:
                return False
            mapping.update({name: val})

    # for key in mapping:
    #     print(key + " -> " + str(mapping[key]))

    stack = []

    with open(formula_path, "r") as formula_file:
        for line in formula_file:
            for word in line.split():
                # print(stack)
                if word == '&&':
                    rop = stack.pop()
                    lop = stack.pop()
                    stack.append(lop and rop)
                elif word == '||':
                    rop = stack.pop()
                    lop = stack.pop()
                    stack.append(lop or rop)
                elif word == '!':
                    op = stack.pop()
                    stack.append(not op)
                elif word == '=>':
                    rop = stack.pop()
                    lop = stack.pop()
                    stack.append((not lop) or rop)
                elif word == '<=>':
                    rop = stack.pop()
                    lop = stack.pop()
                    stack.append(lop == rop)
                else:
                    if (word not in mapping):
                        return False
                    else:
                        stack.append(mapping[word])

        assert(len(stack) == 1)
        # print(stack[0])
        return stack[0]


def test_unit(tu, test_name):
    global test_bin
    test_bin = tu.join_base(test_bin)
    if not os.path.exists(test_bin):
        return tu.FAILURE("testrunner binary not available")
    run_res = tu.run(test_bin, [test_name])
    return tu.check(run_res, "unit test failed")

def test_solver(tu, test_name):
    global solver_bin
    solver_bin = tu.join_base(solver_bin)
    cat, ex, case = test_name.split('.', 2)
    base_name = tu.join_base('test/data/solver/' + cat + '_' + case)
    args = [base_name + '.in']
    rc, out, err = tu.run(solver_bin, args)

    if 'AddressSanitizer' in err:
        return tu.FAILURE('AddressSanitizer error\n' + err)

    if case.endswith('_sat'):
        if rc == 10:
            ref_name = base_name + '.ref'
            if os.path.isfile(ref_name):
                with open(ref_name, 'r') as ref_file:
                    ref_str = ref_file.read()
                    if err == ref_str:
                        return tu.SUCCESS()
                    else:
                        msg = 'expected in stderr:\n'
                        msg += ref_str + '\n'
                        msg += 'but got:\n'
                        msg += err + '\n'
                        return tu.FAILURE('incorrect model\n'+ msg)
            else:
                return tu.SUCCESS()
        elif rc == 20:
            return tu.FAILURE('false unsat result\n')
        else:
            return tu.FAILURE('application returned with error\n' + err)
    else:
        if rc == 20:
            return tu.SUCCESS()
        elif rc == 10:
            return tu.FAILURE('false sat result\n')
        else:
            return tu.FAILURE('application returned with error\n' + err)

def test_parser(tu, test_name):
    global solver_bin
    solver_bin = tu.join_base(solver_bin)
    cat, ex, case = test_name.split('.', 2)
    base_name = tu.join_base('test/data/parser/' + cat + '_' + case)
    args = ['-p', base_name + '.in']
    rc, out, err = tu.run(solver_bin, args)

    if 'AddressSanitizer' in err:
        return tu.FAILURE('AddressSanitizer error\n' + err)

    if case.endswith('_valid'):
        if rc == 0:
            ref = tu.join_base('test/ref_output/parser/' + cat + '_' + case + '.ref')
            with open(ref, 'r') as ref_file:
                if err == ref_file.read():
                    return tu.SUCCESS()
                else:
                    return tu.FAILURE('incorrect formula\n')
        else:
            return tu.FAILURE('application returned with error\n' + err)
    else:
        if rc == 30:
            return tu.SUCCESS()
        elif rc == 0:
            return tu.FAILURE('invalid input accepted\n')
        else:
            return tu.FAILURE('application returned with wrong error code\n' + err)

def test(test):
    cat, ex, case = test.split('.', 2)

    if not (os.path.isfile(solver_bin) and os.access(solver_bin, os.X_OK)):
        return "'%s' is not a file or not executable" % solver_bin

    if not (os.path.isfile(test_bin) and os.access(test_bin, os.X_OK)):
        return "'%s' is not a file or not executable" % test_bin
    if ex == 'stack' or ex == 'cnf' or ex == 'unit':
        return test_unit(ex, case)
    elif ex == 'parser':
        return test_parser(cat, case)
    else:
        assert ex == 'solver'
        return test_solver(cat, case)


all_tests = { }
for test in tests:
    cat, ex, case = test.split('.', 2)
    if ex == 'stack' or ex == 'cnf' or ex == 'unit':
        all_tests[test] = test_unit
    elif ex == 'parser':
        all_tests[test] = test_parser
    else:
        assert ex == 'solver'
        all_tests[test] = test_solver

timeout_secs = 5

