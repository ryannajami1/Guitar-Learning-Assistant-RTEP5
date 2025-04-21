# Guitar-Learning-Assistant-RTEP5 🎸

<img src="https://github.com/user-attachments/assets/eebc4126-89b1-45b2-9803-2ba0b588f1ac" height=250 width="auto">

<p align="center">
  <img src="https://github.com/user-attachments/assets/68d1c1fa-daec-461f-8109-cb89f2e0c949" alt="Picture1" width="30%" height="auto">
  <img src="https://github.com/user-attachments/assets/571432dc-78cc-451e-ba6d-c03c73951b87" alt="Picture2" width="30%" height="auto">
  <img src="https://github.com/user-attachments/assets/dd3785b3-74ac-4e5b-9522-e93c1fba0464" alt="Picture3" width="30%" height="auto">
</p>

<p align="center">
    <img src="https://github.com/user-attachments/assets/c2dda934-bd13-4ddb-9ca5-312394a84ffa" alt="Gif">
</p>


This repository is for the development of a real-time learning assistant used to help beginner guitarists practice. 

The intention with this product is to enable users to effectively learn basic guitar skills with real-time feedback.

The product works by first prompting the user to play a random chord/note/scale etc... (screen)
If the user plays correctly, the screen will show correct (✓) and moves on to the next random shape
If the user plays incorrectly, the screen will show wrong (X) and briefly show the correct shape. Then it will prompt the same item again.

The device has to react to the signal received from the guitar, and determine if the sound is correct or not.

# Wiki / Report 📖
For more information on the project such as hardware and software, social media links, and developer tips please check out our Wiki:
https://github.com/ryannajami1/Guitar-Learning-Assistant-RTEP5/wiki

# Quick Software Guide 🔠
## Install updates and dependencies (-y say yes to all pop-ups)

```sh
./init.sh -y
```
## Compile
```
./compile.sh
```
## OneTime Manual Web Hosting setup:
<strong>  
  Follow the instructions for [Hosting Angular App](https://github.com/ryannajami1/Guitar-Learning-Assistant-RTEP5/wiki/HostingWebAppUsingNginx)
</strong>


## OneTime Alsa Driver setup:
<strong>  
  Follow the instructions for [ALSA Setup](https://github.com/ryannajami1/Guitar-Learning-Assistant-RTEP5/wiki/Environment-and-Hardware-Setup)
</strong>

## Run the executable
The executable will run automatically when a new terminal is opened. Configure the pi to automatically login to the terminal, and it will start automatically when powered on.
```
exec bash
```
