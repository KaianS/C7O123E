# Controle de Servomotor por PWM

Este repositório contém a implementação da atividade de Controle de Servomotor por PWM, utilizando o microcontrolador **Raspberry Pi Pico W** e o simulador **Wokwi**. A atividade faz parte do estudo da Unidade 4, Capítulo 7 (C7O123E) e segue as diretrizes do Prof. Dr. Ricardo Menezes Prates.

## 🛠 Tecnologias Utilizadas
- **Linguagem:** C
- **Ambiente de Desenvolvimento:** VS Code
- **SDK:** Pico SDK
- **Simulador:** Wokwi

## 🗃️ Descrição da Atividade
O objetivo é simular o controle do ângulo de um **servomotor micro servo padrão** usando o **módulo PWM** do **RP2040**. O código configura a GPIO 22 para gerar um sinal PWM com frequência de 50 Hz e ajusta o ciclo de trabalho para posicionar o servomotor nos ângulos de **0°, 90° e 180°**, além de criar uma movimentação suave entre esses pontos.

## 🔧 Funcionalidades Implementadas
1. **Configuração do PWM** na GPIO 22 com frequência de **50 Hz**.
2. Definição do **Duty Cycle** correspondente para os ângulos:
   - **0°** → 500µs
   - **90°** → 1.470µs
   - **180°** → 2.400µs
3. **Movimentação periódica do servomotor** entre 0° e 180° com incrementos suaves de ±5µs e atraso de 10ms.
4. **Experimento com BitDogLab** usando o LED RGB na GPIO 12.

## 🖥️ Como Executar a Simulação
1. **Clone o repositório**:
   ```bash
   git clone https://github.com/KaianS/C7O123E.git
   cd C7O123E
   ```
2. **Compile e execute o código** no ambiente VS Code com o Pico SDK configurado.
3. **Carregue o código no simulador Wokwi** e observe o comportamento do servomotor.

## 🎥 Demonstração em Vídeo
[![Watch the video](https://img.youtube.com/vi/tSnPvu41Pr4/maxresdefault.jpg)](https://youtu.be/tSnPvu41Pr4)

### [Assista aqui o vídeo demonstração](https://youtu.be/tSnPvu41Pr4)