import subprocess
import os

class bc:
    def __init__(self, mysql_concpp_dir="/home/yoshiki01/Downloads/mysql-connector-c++-9.2.0/"):
        self.mysql_concpp_dir = mysql_concpp_dir

    def build(self):
        #subprocess.run("cmake -B build -Dmysql-concpp_DIR=" + self.mysql_concpp_dir, shell=True)
        subprocess.run("cmake -B build", shell=True)
        subprocess.run("cmake --build build -j $(nproc) --target install", shell=True, check=True)

    @staticmethod
    def generate_grpc_and_pb():
        original_dir = os.getcwd()
        try:
            os.chdir("./message/")
            subprocess.run('protoc --proto_path=proto --grpc_out=. --cpp_out=. --plugin=protoc-gen-grpc=$(which grpc_cpp_plugin) message.proto', shell=True, check=True)
            subprocess.run("mv message*.cc src", shell=True)
            subprocess.run("mv message*.h include/message",shell=True)
            subprocess.run(["tree"])
        finally:
            os.chdir(original_dir)

    @staticmethod
    def clean_grpc_and_pb():
        original_dir = os.getcwd()
        try:
            os.chdir("./message/")
            subprocess.run("rm include/message/* src/*", shell=True)
            subprocess.run(["tree"])
        finally:
            os.chdir(original_dir)

bc.generate_grpc_and_pb()
b=bc()
b.build()

