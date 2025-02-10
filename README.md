# Projeto BitDogLab - Controle de LEDs e Display com Interrupções

Este projeto tem como objetivo demonstrar o uso de diferentes componentes eletrônicos conectados à placa BitDogLab, incluindo uma matriz de LEDs WS2812, um LED RGB, botões e um display SSD1306. O projeto foi desenvolvido para explorar conceitos como interrupções, debouncing, controle de LEDs, comunicação serial e uso de displays gráficos.

## Componentes Utilizados

- **Matriz 5x5 de LEDs WS2812**: Conectada à GPIO 7.
- **LED RGB**: Conectado às GPIOs 11 (vermelho), 12 (verde) e 13 (azul).
- **Botão A**: Conectado à GPIO 5.
- **Botão B**: Conectado à GPIO 6.
- **Display SSD1306**: Conectado via I2C (GPIO 14 e GPIO 15).

## Funcionalidades

### 1. Modificação da Biblioteca `font.h`
- **Adição de Caracteres Minúsculos**: A biblioteca `font.h` foi modificada para incluir caracteres minúsculos. Os novos caracteres foram criados de forma criativa para garantir a legibilidade no display SSD1306.

### 2. Entrada de Caracteres via PC
- **Serial Monitor**: O usuário pode digitar caracteres no Serial Monitor do VS Code.
- **Exibição no Display**: Cada caractere digitado é exibido no display SSD1306.
- **Exibição na Matriz de LEDs**: Quando um número entre 0 e 9 é digitado, um símbolo correspondente é exibido na matriz 5x5 de LEDs WS2812.

### 3. Interação com o Botão A
- **Alternar LED Verde**: Pressionar o botão A alterna o estado do LED RGB Verde (ligado/desligado).
- **Registro da Operação**:
  - **Display SSD1306**: Exibe uma mensagem informativa sobre o estado do LED.
  - **Serial Monitor**: Envia um texto descritivo sobre a operação.

### 4. Interação com o Botão B
- **Alternar LED Azul**: Pressionar o botão B alterna o estado do LED RGB Azul (ligado/desligado).
- **Registro da Operação**:
  - **Display SSD1306**: Exibe uma mensagem informativa sobre o estado do LED.
  - **Serial Monitor**: Envia um texto descritivo sobre a operação.

## Requisitos do Projeto

1. **Uso de Interrupções**: Todas as funcionalidades relacionadas aos botões foram implementadas utilizando rotinas de interrupção (IRQ).
2. **Debouncing**: O tratamento do bouncing dos botões foi implementado via software.
3. **Controle de LEDs**: O projeto inclui o uso de LEDs comuns e LEDs WS2812, demonstrando o domínio de diferentes tipos de controle.
4. **Utilização do Display 128x64**: O uso de fontes maiúsculas e minúsculas no display SSD1306 demonstra o domínio de bibliotecas, o entendimento do princípio de funcionamento do display e a utilização do protocolo I2C.
5. **Envio de Informação pela UART**: A comunicação serial via UART foi utilizada para enviar informações ao Serial Monitor.
6. **Organização do Código**: O código foi estruturado e comentado para facilitar o entendimento.

## Estrutura do Código

O código está organizado da seguinte forma:

- **Bibliotecas**: Inclui as bibliotecas necessárias para o controle dos componentes.
- **Definições de Pinos**: Define os pinos utilizados para cada componente.
- **Variáveis Globais**: Armazena o estado dos LEDs e outras variáveis necessárias.
- **Funções de Interrupção**: Implementa as rotinas de interrupção para os botões.
- **Funções de Debouncing**: Implementa o tratamento do bouncing dos botões.
- **Funções de Controle de LEDs**: Controla o estado dos LEDs RGB e da matriz de LEDs WS2812.
- **Funções de Exibição no Display**: Gerencia a exibição de caracteres e mensagens no display SSD1306.
- **Funções de Comunicação Serial**: Gerencia a comunicação com o Serial Monitor.

## Como Executar o Projeto

1. **Conectar os Componentes**: Conecte os componentes à placa BitDogLab conforme descrito na seção "Componentes Utilizados".
2. **Carregar o Código**: Carregue o código na placa BitDogLab utilizando o VS Code.
3. **Abrir o Serial Monitor**: Abra o Serial Monitor no VS Code para interagir com o projeto.
4. **Interagir com o Projeto**:
   - Digite caracteres no Serial Monitor para exibi-los no display SSD1306.
   - Pressione os botões A e B para alternar os estados dos LEDs Verde e Azul, respectivamente.

## Conclusão

Este projeto demonstra o uso de diferentes componentes eletrônicos e técnicas de programação para criar uma aplicação interativa. Através da implementação de interrupções, debouncing e comunicação serial, o projeto oferece uma base sólida para o desenvolvimento de sistemas embarcados mais complexos.
