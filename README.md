# Author
Organization: Technical University of Kosice (TUKE),

Department: Department of Electronics and Multimedia Telecommunications (DEMT/KEMT),

Faculties: Faculty of Electrical Engineering and Informatics (FEI),

Feld of study: 	Informatics,

Study program: Computer Networks, 

Year: 2021 - 2023,

Author: Bc. Marek Rohač -- MR,

Contact: 
  marek.rohac@student.tuke.sk
  marroh004@gmail.com
         
ALL RIGHTS RESERVED.

# Informations

This repository was created to stored all data connected with a process of preparing Diploma Work.  

# Updates

Last modification: 21.04.2023 - MR

# Contributing
As first download git app for your platform --> https://git-scm.com/downloads

## Git Configure
Global Configuration of git
```bash
    git config --global user.name "Name_of_contributor"
    git config --global user.email mail_of_contributor@example.com
```
Add ssh key to your git account. 
S T E P S
1. Generate keys
```bash
  ssh-keygen -t ed25519 -C "comment"      
```
After command just hit enter (~3x).

2. Translate your public key 
```bash
  cat ~/.ssh/id_ed25519.pub     
```
3. Copy your PK a insert into your Account-->SSH-->Add key

For more informations visit --> https://docs.gitlab.com/ee/ssh/
## Cloning Repository
```bash
git clone git@github.com:mr171hg/DiplomaProject.git
```
## Add & Push new data
```bash
git branch name_of_new_branch
git checkout name_of_new_branch
git add -A
git commit -m "name_of_change"
git push --set-upstream origin name_of_new_branch  
```