# deploy
This directory encompasses all of the Ansible playbook scripts used to deploy our software onto the Ground Systems RPi.

In other words, Ansible is used to provision our Ground Systems RPi system and deploy flight and ground softare to it.

Ansible must be run from Linux (cannot run on Windows natively), so it needs to be run either from Docker or Windows Subsystem for Linux (WSL).

## Installing in WSL

Install Ansible and pywinrm, which is needed for interfacing with Windows Remote Management, with standard pip commands. This assumes Python 3.9 and pip are already installed (they do not come by default in WSL). If you desire to setup this, our recommendation is to follow the following steps:
- Go to the Microsoft Store on your Windows Machine.
- Install the "Windows Terminal", as well as "Python 3.9".
- Open up the Windows Terminal application, and check Python was installed correctly (`python3 --version`).
- Install pip (see [this article](https://phoenixnap.com/kb/install-pip-windows)).

Going back to Ansible and pywinrm, we can install it in the following way:

```
    pip install --user ansible pywinrm
```

If you would prefer the Ansible CLI tools to be in an isolated virtual env, [pipx](https://github.com/pypa/pipx) is a good option.

```
    pipx install --include-deps ansible
    pipx inject ansible pywinrm
```

## RPi Deployment Directory Structure

We maintain [this](https://tree.nathanfriend.io/?s=(%27options!(%27fancy7~fullPath!false~trailingSlash7~rootDot7)~8(%278%27%2Fetc%2F*35-3all%20necessary%20files%2C%20including%3A0tele6Ctel6.conf9BCB.ini9*.service0dhcpcd.conf5%2C%27%2Fprojects5-ares_ground_systems0build4adc%20%7Bcompiled%20binary%7D4server4-app2image2redirect2tc2img4--pnid.png0logs9AresSoftware43complete%20git%20repo%20contents3%27)~version!%271%27)-%20%2005--2.py4-3...40-5%5Cn6graf7!true8source!9430B6anaC%2F4%01CB987654320-) directory structure:

```
.
├── /etc/*.../
│   └── ...all necessary files, including:/
│       ├── telegraf/
│       │   ├── telgraf.conf
│       │   └── ...
│       ├── grafana/
│       │   ├── grafana.ini
│       │   └── ...
│       ├── *.service
│       └── dhcpcd.conf
└── ~/projects/
    └── ares_ground_systems/
        ├── build/
        │   ├── adc (compiled binary)
        │   └── server/
        │       ├── app.py
        │       ├── image.py
        │       ├── redirect.py
        │       ├── tc.py
        │       └── img/
        │           └── pnid.png
        ├── deployment_ledger.txt
        ├── logs/
        │   └── ...
        └── AresSoftware/
            └── ...complete git repo contents...
```

Additionally, to ensure that we know the current version deployed, we also setup a Deployment Ledger on the remote host (Ground Systems RPi), and append to it whenever we perform a new deployment.

