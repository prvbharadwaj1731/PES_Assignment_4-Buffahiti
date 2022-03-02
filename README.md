# PES_Assignment_4-Buffahiti
Final submission in fulfillment of coursework for ECEN5823

This application runs on the FRDM-KL25Z board provided by NXP. It uses the built in RGB LED and touch sensor to create a stoplight for the people of Buffahiti. Development was done using MCUXpresso v11.2.0 IDE. There are two releases: DEBUG and RELEASE. The application functions as given below:

1. STOP(#611E3C), GO(#229622), WARNING(#ffb200) and CROSSWALK(#001030) are the states for the stoplight. The hex codes in the bracket are the respective colors for the states. 
2. Transition between all states are gradual and linear over a 1-second period. 
3. Upon sensing a touch input, the system switches to the CROSSWALK state and executes a blink pattern with a duty cycle of 75% for a period of 10 seconds.
4. The system responds to touch inputs within 100 msec in every state except transitioning into the CROSSWALK state or the CROSSWALK state itself. 

# EXTRA CREDIT 1 : PEER REVIEW
Partner : Bharath Raj Govindraju & Madeleine Monfort
Date and time of review : March 1st 2022, 6pm MST

Code changes made:
1. Fixed bug causing incorrect color states at the end of transitions
2. Added touch functionality with resolution of 100 msec
3. Ensuring all magic numbers and constants are well defined
4. Major change to state machine structure for better code quality
5. Changed SysTick_IRQHandler to include critical section 
