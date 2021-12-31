# Timers :

- **timer0** : 8bits ==> 0 à 255
- **timer1** : 16bits ==> 0 à 65535
- **timer2** : 8bits ==> 0 à 255

# Timer 2 :

**Attention ce qui suit est valable pour le timer2. Les autres fonctionnent relativement de la même manière mais diffèrent sur certains points. Il faut vérifier la doc pour les autres**

### Bases :

**Compteur** : TCNT2

**Prescaler:** voir doc pour valeur des CS2x
- CS20(0) de TCCR2B
- CS21(1) de TCCR2B
- CS22(2) de TCCR2B

**Overflow :** ***TOV2***(0) à 1 de ***TIFR2*** 

**Mode normal Timer :**
- WGM20(0) = 0 de TCCR2A
- WGM21(1) = 0 de TCCR2A
- WGM22(3) = 0 de TCCR2B

**Réinitialiser TOV2** = 1 pas 0

### Interruptions:

**Autoriser interruption timer:**
***TOIE2***(0) à 1 de ***TIMSK2***

**Interruption déclenché quand:**
***TOEI2*** = 1 et ***TOV2*** = 1 (overflow)

**Autoriser interruption générale:** sei()

**Interruption Overflow:** ***ISR(TIMER2_OVF_vect)*** :
Le flag ***TOV2*** est remis à 0 tout seul

### Comparaisons et interruptions

**OCR** (Output Compare Register) => 2 par timer ***OCRxA***, ***OCRxB***

**Comparer timer:**
Lorsque le timer est égal à la valeur donnée dans ***OCR2A***, ***OCF2A***(1) est mis à 1
dans ***TIFR2***. Si interruption active alors
***ISR(TIMER2_COMP1_vect*** sera déclénché.

**Autoriser interruption comparaison:**
***OCIE2A***(1) à 1 de ***TIMSK2***

**Mode CTC:** Timer remis à 0 quand ***OCR2A*** = Timer.

**Activer Mode CTC:**
- ***WGM20***(0) = 0 de ***TCCR2A***
- ***WGM21***(1) = 1 de ***TCCR2A***
- ***WGM22***(3) = 0 de ***TCCR2B***

