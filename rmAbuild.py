import subprocess

commands = [
        "docker rm gateserver-container -f",
        "cd /home/yoshiki01/server/GateServer_new/",
        "pwd",
        "docker build --network host --build-arg http_proxy=http://127.0.0.1:7890 --build-arg https_proxy=http://127.0.0.1:7890 -t gateserver_new . ",
        ]
for cmd in commands:
    try:
        print(f"正在执行命令: {cmd}")
        # 执行命令并捕获输出
        result = subprocess.run(cmd, shell=True, check=True,
                              stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                              text=True)
        print("输出结果:")
        print(result.stdout)
        print("-" * 50)
    except subprocess.CalledProcessError as e:
        print(f"命令执行失败: {cmd}")
        print(f"错误信息: {e.stderr}")
    except Exception as e:
        print(f"执行命令时发生未知错误: {e}")

print("所有命令执行完毕")
