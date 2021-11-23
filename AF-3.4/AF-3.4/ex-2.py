from time import sleep
from random import randint
from threading import Thread, Semaphore


def produtor():
    global buffer
    for i in range(10):
        sleep(randint(0, 2))           # fica um tempo produzindo...
        item = 'item ' + str(i)

        sem_produtor.acquire()
        sem_consumidor.release()
        buffer.append(item)
        print('Produzido %s (ha %i itens no buffer)' % (item, len(buffer)))


def consumidor():
    global buffer
    for i in range(10):
        sem_produtor.release()
        sem_consumidor.acquire()
        item = buffer.pop(0)
        print('Consumido %s (ha %i itens no buffer)' % (item, len(buffer)))
        sleep(randint(0, 2))         # fica um tempo consumindo...


buffer = []
tam_buffer = 3

sem_produtor = Semaphore(tam_buffer)
sem_consumidor = Semaphore(0)

produtor = Thread(target=produtor)
consumidor = Thread(target=consumidor)
produtor.start()
consumidor.start()
produtor.join()
consumidor.join()
