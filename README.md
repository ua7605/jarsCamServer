# C-ITS CAM application
This is a repository containing jar files for a CAM server. This include jars for running it on your local host and for running it on a public server
---

## Table of contents
   1. [Installations](#installations)
   1. [Manual](#manual)


## Installations required for LocalHost
### Install DUST Ubuntu 18.04.5LTS
1. Open a new terminal and download this repository: 
- `git clone https://github.com/ua7605/jarsCamServer.git`
- `cd jarsCamServer`
- `cd localHost`
- `cd DUST-INSTALL`
- $ `sudo dpkg -I dust_2.0.0_amd64.deb`
- The DUST Core has been installed.
3. Install the python DUST plugin
- $ `pip3 install pydust_core-2.0.0-cp36-cp36m-linux_x86_64.whl`
- Done! It has been installed.
4. Install the java DUST plugin
- $ `sudo apt-get install openjdk-8-jdk`
- $ `sudo apt install maven`
- $ `cd dust-java`
- $ `mvn package`
- $ `mvn install`
- Done! The java DUST plugin has been installed.


### Install CAM server for localHost
1. Open a new terminal and go to the jarsCamServer repository:  
- `cd jarsCamServer`
- `cd localHost`
- `cd server`
3. Start the CAM server
- `java -jar camserver-0.0.1-SNAPSHOT.jar`
4. Done! The server is running on your local machine
5. To download the source code of the cam server localHost version: open a new terminal window
- `git clone https://github.com/ua7605/camserver.git`
- `git checkout localHost`
6. Done! You have the source code
### Go to the web application 
1. Open a new browser window: and put the following in your URL:
- `http://localhost:8080/`
2. To go to the web application service go to the following tab
- `http://localhost:8080/ServiceCAM` 
3. Done! you will see now the web appication 
### Run demo CAM message generator 
1. Open a new terminal form the root repo
- `cd jarsCamServer`
- `cd localHost`
- `cd MessageGeneratorDemo`
2. Start the demo special vehicle CAM message generator
- `java -jar CAM_message_SPV_Gen_Demo.jar` 
3. Done! The demo messages will be sent 
