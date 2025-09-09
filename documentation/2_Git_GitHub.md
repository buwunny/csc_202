<br>

<div style="display: flex; align-items: center; gap: 20px;">
  <div style="font-size: 2em; font-weight: bold;">
  Configuring CCS-Theia and GitHub
  </div>
  <div style="position: absolute; top: 20px; right: 50px;">
  <a href="../Readme.md" title="Go back" style="text-decoration: none; font-size: 32px;">↩</a>
  </div>
</div>
<br>
<hr style="border: none; border-bottom: 4px solid #FFF; width: 100%; margin-top: 0;">

> Version 1.3  
> Date: Fall 2025

---
## 🎯 Introduction
To fully integrate CCS-Theia with Git and GitHub, you will push your local repository from your machine to GitHub. Below are the instructions for pushing and pulling files to/from GitHub.

---

<br>

## ⚙️ Configure Git Global Configurations

Before you can use Git, you must define your global user information. This is necessary because Git tracks your identity for every commit you make. Setting up this configuration ensures that each commit has the correct author information attached to it.

 **Open a new terminal in CCS-Theia**
1. Enter the following command to set your e-mail address
    ```bash
    git config --global user.email "{your-email-address}"
    ```
     - Ensure that you replace the `{your-email-address}` with your e-mail address.
     - This sets the email address that will be associated with your commits. This is especially important for identifying who made a particular change when collaborating on a project.
   
2. Enter the following command to set your username
    ```bash
    git config --global user.name "{your-name}"
    ```
     - Ensure you replace the `{your-name}` with your name.
     - This sets the name that will be associated with your commits. This helps identify who made the changes, which is especially useful for collaboration in shared repositories.

⚠️ NOTE: Using the `--global` flag, Git applies this configuration to every repository on your system. If you want to override the global configuration for a specific repository, you can set local configurations (specific to that repo) by omitting the `--global` flag and running the same commands within the repository folder.

---

<br>

## 📤 Pushing from CCS-Theia to GitHub

Once you have created a repository on your local machine using CCS-Theia, you must connect it to a remote repository on GitHub. You only need to set up this connection once, and CCS-Theia will remember it for future use.

 **Open a new terminal in CCS-Theia**

1. Configure your Git global configurations for user name and email, if they aren't already configured.

2. Connect your local repository to the remote repository on GitHub by running this command:
    ```bash
    git remote add origin https://github.com/{your-git-hub-username}/{name-of-repo}.git
    ```
     - Replace the `{your-git-hub-username}` with your GitHub account name.
     - Replace the `{name-of-repo}` with your repository name.

3. Push your local repository to GitHub:
    ```bash
   git push -u origin master
    ```
    - The `-u` flag in the `git push` command stands for **"upstream"**. It tells Git to remember the remote repository (in this case, `origin`) and the branch (in this case, `master`) that you're pushing to. This establishes a tracking relationship between your local and remote branches, simplifying future Git commands.

  ⚠️ NOTE: Do not create files directly on GitHub before pushing your local repository. If you do, GitHub will create a branch called `main`. This can cause confusion and result in two branches when you push from your local machine.

---

<br>

## 📥 Pulling from GitHub to a New CCS-Theia Setup

If you have an existing repository on GitHub and want to work on it from a different machine (e.g., a new PC or a work/school computer), you'll need to pull the repository from GitHub to your new machine. Follow these steps to set up the repository on your new computer and access your files:

1. **On the new machine**, create a folder where you want to store the repository. This will serve as the location for the files you pull from GitHub.

2. **In CCS-Theia**, open the newly created folder to make it your workspace. This sets up your environment for the new project.

3. **In the Source Control tab**, initialize the local repository to setup Git for the workspace.

4. **Open a new terminal in CCS-Theia** and issue the following commands:
   - Git Global Configurations, if they aren't already configured on the new machine.
   - Add the GitHub repository as the remote origin:
    ```bash
     git remote add origin https://github.com/{your-git-hub-username}/{name-of-repo}.git
    ```
      -   Replace the `{your-git-hub-username}` with your GitHub account nameand `{name-of-repo}` with your repository name.

   - Pull the latest changes from the GitHub repository to your local machine. Replace `master` with `main` if that is your default branch on GitHub:
    ```bash
     git pull origin master
    ```

This will download the latest repository version from GitHub to your local machine, so you can start working on it in CCS-Theia.

---

<br>

## 📞 Contact Information
Bruce Link  
Monroe Community College  
Information & Computer Technologies Department  
[blink@monroecc.edu](mailto:blink@monroecc.edu)


