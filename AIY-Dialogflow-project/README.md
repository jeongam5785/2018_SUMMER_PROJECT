-------AIY-Dialogflow-project with raspberry pi zero w in Voice Kit-------

The purpose of this project is to control the home appliances with voice recognition. Our team updraded normal home appliances to smart home appliances, such as, electronic fan, light, humidifier, TV. So, by using a dialogflow API and a google assistant API, We can give an order to Voice Kit in a voice and the updraded home appliances operate according to order. 

You have to go through a few steps to use this sources.

1-1. #install dialogflow / sudo pip3 install dialogflow

1-1. #install paho-mqtt / sudo pip3 install paho-mqtt

2-1. #create dialogflow agent / https://dialogflow.com/

2-2  #download the credentials 

![real](https://user-images.githubusercontent.com/39085495/43694573-efe2dc68-996e-11e8-8155-e3d4fadf0ca4.PNG)

2-3  #copy the text and add assistant.json file in rasp board / $sudo nano assistant.json 

2-4  #download the service account ket

![first](https://user-images.githubusercontent.com/39085495/43696675-2313e00e-997a-11e8-98a1-d662359f8be0.PNG)
![second](https://user-images.githubusercontent.com/39085495/43696684-2f0117e2-997a-11e8-93ed-2fbfa7458653.PNG)

