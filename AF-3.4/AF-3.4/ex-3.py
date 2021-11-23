from multiprocessing import Process, Queue

DEFAULT_ENCODING = "utf-8"

def origem(fila1: Queue):
    for i in range(10):
        msg = 'Teste ' + str(i)
        msg_as_bytes = msg.encode(DEFAULT_ENCODING)
        fila1._send_bytes(msg_as_bytes, 0, len(msg_as_bytes))  # envia mensagem para a ponte pela fila 1
        print('Mensagem enviada:', msg)


def ponte(fila1: Queue, fila2: Queue):
    for i in range(10):
        received_bytes = fila1._recv_bytes()  # recebe mensagem na fila 1
        fila2._send_bytes(received_bytes, 0, len(received_bytes))  # reenvia mensagem na fila 2
        msg = received_bytes.decode(DEFAULT_ENCODING)
        print('Mensagem encaminhada:', msg)


def destino(fila2: Queue):
    for i in range(10):
        received_bytes = fila2._recv_bytes()  # recebe mensagem na fila 2
        msg = received_bytes.decode(DEFAULT_ENCODING)
        print('Mensagem recebida:', msg)


if __name__ == '__main__':
    fila1 = Queue()
    fila2 = Queue()
    o = Process(target=origem, args=(fila1,))
    p = Process(target=ponte, args=(fila1, fila2))
    d = Process(target=destino, args=(fila2,))

    o.start()
    p.start()
    d.start()

    o.join()
    p.join()
    d.join()

    fila1.close()
    fila2.close()
