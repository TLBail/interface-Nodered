int autonomie = 1; // va de 1 a 3

String bob = ".";
void enterAutonomie(){
    drawstyles(bob+ " Qu'elle as été votre autonomie lors de cette atelier ? \n  : " + autonomie );


}


void incValueAutonomie(){
    if(autonomie >= 3){
        autonomie = 0;
    }else{
        autonomie++;
    }

}
