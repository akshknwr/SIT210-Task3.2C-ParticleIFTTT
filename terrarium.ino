
/* light reading system to calculate the duration of light for plant and get notifcation via IFTTT */
int lightSensorPin = A0; // to get input from lightsensor
int analogValue = 0; 
int greenLed=D6; // for indicating when light is receiving

int sunriseNotified=0; // to make note whether user has notified about sun exposure or not
int sunsetNotified=0; // to make note whether user has notified about light being gone or not
//start time
int startHour=0;
int startMinute=0;
//finish time
int finishHour=0;
int finishMinute=0;
//total time
int totalHour=0;
int totalMinute=0;




void setup() {
    pinMode(greenLed,OUTPUT);
      Particle.variable("analogvalue", &analogValue, INT); // registering for particle variable
      Particle.function("manualValue",lightsensor); // registering for particle function to access from cloud
      Time.zone(11);
    

}

void loop() {
    analogValue = analogRead(lightSensorPin);
    
    if (analogValue >= 100) { // test whether sensor is reading enough light for plant
      //let the user know about light amount with Green LED on      
    digitalWrite(greenLed, HIGH);
        
       if (sunriseNotified==0){  // test if user has notified about it in order to avoid constant notification about same event
        digitalWrite(greenLed,HIGH); // green led turning on will indicate its reading efficient amount of light
        
        //time of starting reading light
    startHour=Time.hour(Time.now());
    startMinute=Time.minute(Time.now());
    

    //publish on particle
    String startNotice= "Sun has started nurturing your plant at "+String(startHour)+":"+String(startMinute);
    
      Particle.publish("terarium",startNotice);
  
    
    // now next time no need to send notification in loop
        sunriseNotified =1;
     // value of light amount.      
    Particle.publish("lightValue",String(analogValue));
        
    } else if (analogValue <100 ){
         // when light starts going away from plant turn off the LED
        digitalWrite(greenLed,LOW);
        
        if(sunsetNotified=0){
        sunsetNotification(); // send notifcation for summary
       
      
    }
    }
    
    }
   //check every 5 minutes 
delay(50000);
 
 
  

}
//to test the summary from particle cloud
int lightsensor(String analogvalue){
    sunsetNotification();
    
}


//send the calculated time to particle
void sunsetNotification(){
     Particle.publish("lightVlaue", String(analogValue));
     
    //   finishHour=Time.hour(Time.now());
    //   finishMinute=Time.minute(Time.now());
  
        finishHour= Time.hour(Time.now());
        finishMinute = Time.minute(Time.now());
        totalHour=finishHour - startHour;
        if(startMinute > finishMinute){
            totalMinute = startMinute - finishMinute;
        } else if (startMinute < finishMinute){
            totalMinute = finishMinute - startMinute;
            
        } else totalMinute = 0;
        
        String totalTime =" sun started from "+ String(startHour)+":"+String(startMinute)+" till "+ String(finishHour)+":"+String(finishMinute)+" and got light for "+String(totalHour)+" Hours and "+String(totalMinute)+" Minutes in total.";
        
        Particle.publish("terarium",totalTime);
        
       
        sunsetNotified =1;
}