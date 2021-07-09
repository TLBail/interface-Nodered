int actualMachineIndex = 0;
int valueOfActualMachine = 0; // va de 0 a 100
void enterMachineInfo(){
    drawstyles( " entrer la valeur de " + machineToString(selectedMachine[actualMachineIndex])  + "\n incremente(bouton gauche)\nsuivant(bouton droit)\n valeur : " + valueOfActualMachine);
}


void incValueMachineInfo(){
    if(valueOfActualMachine >= 100){
        valueOfActualMachine = 0;
    }else{
        valueOfActualMachine += 5;
    }

}

void incMachineIndex(){
    if(selectedMachine[actualMachineIndex + 1] != 0){
        machinesInfo[actualMachineIndex].value = valueOfActualMachine;
        actualMachineIndex++;
    }else{
        machinesInfo[actualMachineIndex].value = valueOfActualMachine;
        stateIndex++;
        generateMachinesInfo();
    }
}



void generateMachinesInfo(){

   DynamicJsonDocument  doc(2048);
    JsonObject root = doc.createNestedObject();


    int i = 0;
    while(selectedMachine[i] != 0){
        machinesInfo[i].machineType = selectedMachine[i];
        

        char* objectName = "0";
        ultoa(i , objectName , 10);
        JsonObject machine = root.createNestedObject(objectName);
        machine["name"] = machineToString(selectedMachine[i]);
        machine["value"] = machinesInfo[i].value;
        i++;
    }

    root["autonomie"].set(autonomie);
    
    String output = "";
    serializeJson(root, output);


    MQTTclient.publish("esp32/machinesInfo", output.c_str());
  

}



String machineToString(Machine machineToProccess){

    for (size_t i = 0; i < NBMACHINE; i++)
    {
        if(machineToProccess == machines[i]){
            return machineText[i];
        }
    }
    

    return "";
}
