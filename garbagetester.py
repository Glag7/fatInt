import sys
import re

def main():
    data = sys.stdin.read().splitlines()
    tests = []
    current_test = None
    current_section = None
    for line in data:
        line = line.strip()
        if line.startswith("TEST"):
            if current_test is not None:
                tests.append(current_test)
            current_test = {'add': [], 'sub': [], 'sub rev': []}
            current_section = None
        elif line in ['add', 'sub', 'sub rev']:
            current_section = line
        elif line and current_section is not None:
            current_test[current_section].append(line)
    if current_test is not None:
        tests.append(current_test)
    
    max_uint64 = (1 << 64) - 1  # 18446744073709551615
    
    for test_index, test in enumerate(tests):
        for section, lines in test.items():
            for line in lines:
                parts = line.split(' = ')
                if len(parts) < 2:
                    continue
                left = parts[0]
                right = parts[1].strip()
                
                numbers = re.findall(r'\(\d\)(-?\d+)', left)
                if len(numbers) != 2:
                    continue
                try:
                    a = int(numbers[0])
                    b = int(numbers[1])
                except:
                    continue
                
                if section == 'add':
                    res = a + b
                elif section == 'sub':
                    res = a - b
                elif section == 'sub rev':
                    res = a - b  # Corrected: now a - b instead of b - a
                else:
                    continue
                
                match = re.match(r'\((\d)\)(-?)(.*)$', right)
                if not match:
                    continue
                sign = match.group(2)
                value_str = match.group(3)
                expected_str = sign + value_str
                
                if expected_str == 'too big':
                    if res <= max_uint64:
                        print(f"TEST {test_index} failed: {line}")
                        print(f"Expected: {right}, Got: {res} (which is within range)")
                elif expected_str == '-too big':
                    if res >= -max_uint64:
                        print(f"TEST {test_index} failed: {line}")
                        print(f"Expected: {right}, Got: {res} (which is within range)")
                else:
                    try:
                        expected_value = int(expected_str)
                    except:
                        continue
                    if res != expected_value:
                        print(f"TEST {test_index} failed: {line}")
                        print(f"Expected: {res}, Got: {right}")
                    else:
                        if res > max_uint64 or res < -max_uint64:
                            print(f"TEST {test_index} failed: {line}")
                            print(f"Expected: {right}, but result {res} is out of range and should be reported as too big")

if __name__ == '__main__':
    main()
