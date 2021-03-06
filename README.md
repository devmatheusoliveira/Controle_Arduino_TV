# Controle seu teclado da SMART TV LG com Arduino

*Atenção, este produto está em fase de teste, por isso não nos responsabilizamos por qualquer dano causado em seus equipamentos*

## Sobre o projeto:
* Quem usa Smart TV sabe quão cansativo é digitar alguma coisa usando o controle remoto. Além de demorado, é pouco eficaz. Cansado disso, decidi implementar uma solução usando Arduino. 
* Ainda está em fases de teste mas tem me deixado bastante satisfeito.
---

## Tecnologias utilizadas:

* Sistema embarcado:
  *  Arduino, 
  *  Linguagem: C++
* Aplicativo:
  *  Framework: Flutter
  *  Linguagem: Dart
* Desenho/modelagem do case:
  *  AutoCad
  *  Scketchup
---
## Materiais utilizados:

| Quantidade | Componente | Descrição|
| --- | --- | ---|
| 1 | LED Infravermelho | Responsável por enviar comandos em IR |
| 1 | Resistor de 220 ohms | Evita que o LED queime |
| 1 | Arduino UNO | Micro-controlador para unir tudo | 
| 1 | Modulo Bluetooth | Faz a comunicação Bluetooth com o Smartphone |

---

## Lembretes importantes
* Versão da biblioteca: 2.0.1
* A porta que liga o LED Infravermelho geralmente é o pino digital 3, mas pode variar conforme a placa que estiver usando, consulte a documentação oficial
* Este código está em versão inicial, por isso pode apresentar alguns bugs
* Fique a vontade para contribuir com o repositório!
---
