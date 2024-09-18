import random
import os

def generate_random_floats(n, lower=0.0, upper=10.0):
    """
    生成 n 个在 [lower, upper) 范围内的随机浮点数。
    :param n: 生成的随机数个数
    :param lower: 随机数的下限（包含）
    :param upper: 随机数的上限（不包含）
    :return: 一个包含 n 个随机浮点数的列表
    """
    random_floats = [random.uniform(lower, upper) for _ in range(n)]
    return random_floats

def save_random_floats_to_file(random_floats, filename):
    """
    将随机浮点数列表保存到文件中。
    :param random_floats: 随机浮点数列表
    :param filename: 文件名
    """
    with open(filename, 'w') as file:
        for num in random_floats:
            file.write(f"{num}\n")

n = 10  # 生成 10 个随机浮点数
filename = "../src/random_numbers.txt"  # 输出文件名

# 生成随机浮点数
random_numbers = generate_random_floats(n)

# 保存到文件
save_random_floats_to_file(random_numbers, filename)

print(f"Generated {n} random floats and saved to {filename}")