# Trabalho 1 da matéria Redes de Computadores I

## Streaming de video.

Modo de uso:


Primeiro você deve inicializar em um terminal o servidor.
```bash
make server
```

Depois você deverá inicializar o client.
```bash
make client
```

Assim você estabelecerá a conexão entre eles.


Mensagem à ser transmitida:

| Marcador de Inicio | Tamanho em bytes | Sequência | Tipo | Dados | CRC-8 |
|:------------------:|:----------------:|:---------:|:----:|:-----:|:-----:|
| 8 bits             | 6 bits           | 5 bits    | 5 bits| 0-64 bytes | 8 bits |
| 1 byte             |        ->        | 2 bytes   |  <-   | 0-64 bytes | 1 byte |


O marcador de inicio sempre será `0111 1110`

O campo tipo pode ser:

| bits | código |
|:----------:|:---:|
|`00000` | ack|
|`00001` | nack|
|`01010` | lista|
|`01011` | baixar|
|`10000` | mostra na tela|
|`10001` | descritor arquivo|
|`10010` | dados |
|`11110` | fim tx |
|`11111` | erro|

Os campos de erro podem estar com a seguinte mensagem em dados:

| erro |  definição |
|:----:|:----------:|
| `1` | acesso negado |
| `2` | não encontrado|
| `3` | disco cheio   |