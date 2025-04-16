# Guitar-Learning-Assistant-RTEP5
![image](https://github.com/user-attachments/assets/e07a5550-07ae-4ea3-8d5d-35c13ffcd222)

This repository is for the development of a real-time learning assistant used to help beginner guitarists practice. 

The intention with this product is to enable users to effectively learn basic guitar skills with real-time feedback.

The product works by first prompting the user to play a random chord/note/scale etc... (screen)
If the user plays correctly, the screen will show correct (✓) and moves on to the next random shape
If the user plays incorrectly, the screen will show wrong (X) and briefly show the correct shape. Then it will prompt the same item again.

The device has to react to the signal received from the guitar, and determine if the sound is correct or not.

# WIKI
For more infromation on the project such as hardware and software, social media links, and developer tips please check out our WIKI:
https://github.com/ryannajami1/Guitar-Learning-Assistant-RTEP5/wiki

## Setup instructions
### 1. Install WSL (Windows only)
Go to the microsoft store and search for WSL or Ubuntu and install Ubuntu
![alt text](images/Ubuntu.png)

An instance of WSL can be opened by searching and running Ubuntu from the windows search bar, or by typing wsl is cmd or powershell.

On opening for the first time there will be some setup - it asks you to setup an account and create a user password iirc but thats pretty much it.

This link provides more information on installing WSL https://learn.microsoft.com/en-us/windows/wsl/install
### 2. Install dependencies
Run the following commands in wsl to install the correct dependencies:
```
sudo apt update
sudo apt upgrade
sudo apt-get install clang
sudo apt-get install gcc
sudo apt-get install gdb
sudo apt-get install clang-tidy
```
### 3. Build the repository
After cloning the repo it can be built by running build.sh
```
./build.sh
```
