# Linux_Study
include linux_basic \ linux_process \ linux_thread \ linux_socket


# 在GitHub 上创建一个仓库后，从Linux上传文件的步骤

# 1、在Linux命令行获取 ssh 密钥
ssh-keygen -t rsa -b 4096 -C "your emaill@example.com"

# 2、在 GitHub 上配置公钥
cat ~/.ssh/id_rsa.pub //打开文件，复制公钥

打开GitHub上的setting，找到 SSH and GPG keys，点击 New SSH keys，复制公钥，点击 Add SSH Key。

# 3、在 Linux 上配置 SSH
touch ~/./ssh/config
vim ~/./ssh/config
在 config 中添加内容：配置私钥
Host github.com
  HostName github.com
  User git
  IdentityFile ~/.ssh/id_rsa

# 4、测试连接
ssh -T git@github.com

# 5、在 GitHub 上复制仓库的 SSH 地址

# 6、clone 克隆仓库
git clone SSH_address

# 7、把需要上传的文件复制到本地仓库
cp -r local_file_address local_repository_address

# 8、上传文件
// 在本地仓库的目录下
git remote -v // 查看origin连接
git remote rm origin // 删除origin连接

git branch // 查看本地分支
git branch -a // 查看远程仓库分支
git checkout -b new_branch_name // 切换分支
git branch -d branch_name // 删除分支

git init // 初始化一个 .git
git remote add origin SSH_address // 创建origin连接
git add <folder_name> // 上传指定的文件夹       {  git add . // 上传当前路径  }
git commit -m "ADD <folder_name>" // 上传的文字说明 add\update\first commit等
git push origin main
