import re
import os
import time
import datetime
import subprocess
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches


# 获取当前时间并格式化为字符串
now = datetime.datetime.now()
timestamp = now.strftime("%Y%m%d_%H%M%S")
# 定义程序和配置文件
config_files = ["none"]
programs = ["./build/test_mallocator", "./build/test_stdallocator"]

colors = ['royalblue', 'orangered', 'dodgerblue', 'blueviolet', 'maroon', 'crimson', 'teal', 'olive', 'cyan', 'brown', 'royalblue']
num_runs = 40  # 运行次数 N

# 用于存储结果
results = {config: {prog: {} for prog in programs} for config in config_files}

# 运行程序并收集数据
for config in config_files:
    for prog in programs:
        gflops_results = []
        for _ in range(num_runs):
            command = [prog]
            result = subprocess.run(command, capture_output=True, text=True)
            output = result.stdout.strip().split('\n')[-1]
            # gflop_value = re.findall(r"\(\s*([\d.]+)\s*GFlops\)", output)
            run_time = float(re.findall(r"[\d.]+", output)[0])
            if run_time:
                gflops_results.append(run_time)
                print(datetime.datetime.now(), f"{prog} {output} -> {run_time}")
            else:
                print(datetime.datetime.now(), f"{prog} {output}")

            time.sleep(0.1)

        if gflops_results:
            results[config][prog] = {
                'max': np.max(gflops_results),
                'mean': np.mean(gflops_results),
                'min': np.min(gflops_results)
            }

# 生成柱状图
fig, ax = plt.subplots(figsize=(len(programs) * len(config_files) + 3, 10))
group_width = 0.8
bar_width = group_width / len(programs)
index = np.arange(len(config_files))

for i, prog in enumerate(programs):
    max_values = [results[config][prog]['max'] for config in config_files]
    mean_values = [results[config][prog]['mean'] for config in config_files]
    min_values = [results[config][prog]['min'] for config in config_files]
    rects1 = ax.bar(index + i * bar_width, max_values, bar_width, label=prog, color=colors[i])
    bars = ax.bar(index + i * bar_width, mean_values, bar_width, edgecolor='snow',color=colors[i], hatch='', linewidth=0)
    bars_min = ax.bar(index + i * bar_width, min_values, bar_width, edgecolor='grey',color=colors[i], hatch='', linewidth=0)

    # 添加数据标签
    for rect, max_val, mean_val, min_val, bar, mbar in zip(rects1, max_values, mean_values, min_values, bars, bars_min):
        bx = bar.get_x()
        bwidth = bar.get_width()

        ax.add_patch(patches.Rectangle((bx, bar.get_height()), bwidth, min_val / 200, color='whitesmoke', fill=None, lw=1, linestyle='dashed'))
        ax.add_patch(patches.Rectangle((bx, mbar.get_height()), bwidth, min_val / 200, color='dimgrey', fill=None, lw=1, linestyle='dashed'))

        ax.annotate(f'{mean_val:.6f}', xy=(rect.get_x() + rect.get_width() / 2, bar.get_height()),
                    xytext=(0, 3), textcoords="offset points", ha='center', va='bottom', color='white')
        ax.annotate(f'{min_val:.6f}', xy=(rect.get_x() + rect.get_width() / 2, mbar.get_height()),
                    xytext=(0, 3), textcoords="offset points", ha='center', va='bottom', color='silver')
        ax.annotate(f'{max_val:.6f}', xy=(rect.get_x() + rect.get_width() / 2,  rect.get_height()),
                    xytext=(0, 3), textcoords="offset points", ha='center', va='bottom')


# 设置图表属性
ax.set_xlabel('Input File')
ax.set_ylabel('Time / s')
ax.set_title(f'BST Performance Runs: {num_runs} start: {timestamp} end: {datetime.datetime.now().strftime("%Y%m%d_%H%M%S")}')
ax.set_xticks(index + group_width / 2 - bar_width / 2)
ax.set_xticklabels(config_files)
ax.legend()

# 调整布局并保存图像
fig.tight_layout()
plt.savefig(f'./result/autobench_{timestamp}.png', dpi=300)

print(results)
