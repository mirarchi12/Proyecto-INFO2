# Proyecto-INFO2
Secuenciador/Caja de ritmo para ESP-32

En sensor:
  El umbral de ruido se ecuentra en el primer if del slot sensorPiezo, asi como tambien, el control de rebote se encuentra en el ultimo if.

En player:

  Tanto los archivos de sonido como el directorio que los contiene se configuran en el constructor de “player”.
Pueden tener cualquier nombre siempre y cuando los sonidos sean del tipo “*.wav” y posean un numero valido (1-8) que indique a que piezoeléctrico corresponde.
Para qPow: El exponente γ (Segundo argumento) controla la relación entre intensidad de golpe y volumen (γ=1 -> Lineal ; γ=0 -> Volumen Constante).

En general: 
  Existe un pequeño delay acumulativo entre cada reproduccion de una secuencia, lo que va desfasandola respecto al BPM establecido; Aun no se conoce la causa :(.
