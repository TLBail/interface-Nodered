#define NBOFPAGE 5

int actualScreenIndex = 1;  // de 1 a NBOFPAGE

bool screenHaveChanged = true;

int nbMachineAdded = 0;



void displayScreen(){    
    drawstyles("Avez vous utilisé  \n " + machineText[actualScreenIndex - 1] + " ? \n bouton gauche pour confirmé \n bouton droitpour infirmé ");
}



void incMachineScreenIndex(){
    if(actualScreenIndex >= NBOFPAGE){
        actualScreenIndex = 1;
    }else{
        actualScreenIndex++;
    }
}

void decMachineScreenIndex(){
    if(actualScreenIndex <= 1){
        actualScreenIndex = NBOFPAGE;
    }else{
        actualScreenIndex--;
    }
}

void selectMachine(){
    selectedMachine[nbMachineAdded] = machines[actualScreenIndex - 1];
    nbMachineAdded++;
}