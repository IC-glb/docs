# Git
---
## 初始配置
登录--->使用--global表示所有的项目都会默认使用这里配置的用户信息
~~~ shell

$ git config --global user.name "icccc8888"
$ git config --global user.email 2513554233@qq.com
~~~
>这是gitee的账号

---
## 创建和管理仓库
1.先创建一个仓库
~~~shell
mkdir Test
cd Test
~~~
2.在当前目录新建一个Git代码库
~~~shell
git init

Initialized empty Git repository in 路径/.git/
~~~
3.添加文件
~~~shell
git add .   #添加所有文件
~~~

4.提交前要对此次提交进行说明
~~~shell
git commit -m 'notes'
~~~

5.提交
~~~shell
git push
~~~

6.拉仓库
~~~shell
#克隆：
git clone xxx.git

#进入仓库
git pull

~~~

---
## 基本操作

### 查看
~~~shell
git status
#会告诉我们什么文件被修改过


git log
#命令显示从最近到最远的提交日志


git log --pretty=oneline
#显示从最近到最远提交的 commit id（版本号）
~~~


---
### 版本回退
在Git中，用HEAD表示当前版本，也就是最新的提交4d8facc...，上一个版本就是HEAD^，上上一个版本就是HEAD^^，当然往上100个版本写100个^比较容易数不过来，所以写成HEAD~100。

使用命令**git reset --hard commit_id** 回退到指定的版本。



## 分支
### 创建分支
git checkout -b创建 dev 分支，并切换到 dev 分支
~~~shell
$ git checkout -b dev
Switched to a new branch 'dev'
~~~

git branch查看当前分支，git branch 命令会列出所有分支，当前分支前面会标一个*号。
~~~shell
$ git branch
* dev
  master
~~~

### 合并分支
dev 分支的工作完成，切换回 master 分支，此时的 readme 并没有发生改变
~~~ shell
$ git checkout master
Switched to branch 'master'
Your branch is up to date with 'origin/master'.

$ cat readme
Git is a distributed version control system.
Git is free software distributed under the GPL.
Git has a mutable index called stage.
Git tracks changes.
~~~

把dev分支的工作成果合并到master分支上，现在和dev分支的最新提交是完全一样的

这里的Fast-forward信息，Git告诉我们，这次合并是“快进模式”，也就是直接把master指向dev的当前提交，所以合并速度非常快。

~~~shell
$ git merge dev
Updating 27f0555..3cd7f88
Fast-forward
 readme | 3 ++-
 1 file changed, 2 insertions(+), 1 deletion(-)

$ cat readme
Git is a distributed version control system.
Git is free software distributed under the GPL.
Git has a mutable index called stage.
Git tracks changes.
branch test

~~~
合并完成后，可以git branch -d 命令删除 dev 分支，此后只有 master 一个分支

~~~shell

$ git branch -d dev
Deleted branch dev (was 3cd7f88).

$ git branch
* master
~~~


### 查看远程库信息
- git remote 可查看简略信息
- git remote -v 可查看详细信息，包括可以抓取和推送的 origin 的地址

~~~ shell
$ git remote
origin

$ git remote -v
origin  https://github.com/dreamwhigh/learngit.git (fetch)
origin  https://github.com/dreamwhigh/learngit.git (push)

~~~

### 推送分支
推送分支，就是把该分支上的所有本地提交推送到远程库。推送时，要指定本地分支，这样，Git就会把该分支推送到远程库对应的远程分支上。

~~~shell
$ git push origin master

$ git push origin dev
~~~
master分支是主分支，因此要时刻与远程同步；
dev分支是开发分支，团队所有成员都需要在上面工作，所以也需要与远程同步；
bug分支只用于在本地修复bug，就没必要推到远程了，除非老板要看看你每周到底修复了几个bug；
feature分支是否推到远程，取决于你是否和你的小伙伴合作在上面开发。


---

## 标签
### 创建标签
发布一个版本时，我们通常先在版本库中打一个标签（tag），这样，就唯一确定了打标签时刻的版本。将来无论什么时候，取某个标签的版本，就是把那个打标签的时刻的历史版本取出来。所以，标签也是版本库的一个快照。

Git的标签虽然是版本库的快照，但其实它就是指向某个commit的指针。
- 命令git tag <tagname>用于新建一个标签，默认为HEAD，也可以指定一个commit id；
- 命令git tag -a <tagname> -m "blablabla..."可以指定标签信息；
- 命令git tag可以查看所有标签。
---
### 操作标签
- 命令git push origin <tagname>可以推送一个本地标签；
- 命令git push origin --tags可以推送全部未推送过的本地标签；
- 命令git tag -d <tagname>可以删除一个本地标签；
- 命令git push origin :refs/tags/<tagname>可以删除一个远程标签。