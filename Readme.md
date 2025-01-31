# Temporizador de disparo único

## Descrição

Este repositório contém os códigos correspondentes aos requisitos solicitados pela tarefa 1 do capítulo 5 da unidade 4 do programa de capacitação em sistemas embarcados (Embarcatech). A tarefa em questão busca incitar a utilização de um temporizador de disparo único para alterar o estado de um LED RGB ao detectar o acionamento de um botão.

## Requerimentos

O presente projeto deverá atender aos seguintes requerimentos:

* Caso o usuário clique no botão (pushbutton), os três LEDs serão ligados (todos em nível alto). A partir da primeira rotina de atraso, ocorrerá uma mudança de estado para dois LEDs ligados e, em seguida, apenas um.
* O temporizador do alarme deve ser ajustado para um atraso de 3 segundos (3.000ms), entre os estados de acionamento dos LEDs.
* A mudança de estado dos LEDs deve ser implementa em funções de call-back do temporizador, a exemplo da rotina trabalhada na aula síncrona - turn_off_callback().
* O botão só pode alterar o estado dos LEDs quando o último LED for desligado. Deste modo, durante a execução das rotinas de temporização, o botão não pode iniciar uma nova chamada da função call-back.
* Implementar uma rotina em software para atenuação do efeito bouncing no botão (software debounce).  

## Demonstração

Seguem os vídeos de demonstração do projeto, tanto no simulador Wokwi quanto na placa BitDogLab.

## Instruções

Para que o projeto funcione em sua máquina, é necessário ter a extensão Raspberry Pi Pico Project instalada no VSCode.

1. Realize o clone do repositório.
2. Importe o repositório como um projeto através da extensão Raspberry Pi Pico Project.
3. Compile o projeto.
4. Caso esteja usando a placa BitDogLab ou a Raspberry Pi Pico W, faça o upload do arquivo uf2, encontrado na pasta `build` após o processo de compilação, para a placa. Caso deseje simular, abra o arquivo `diagram.json` (É necessário ter a extensão do Wokwi instalada).