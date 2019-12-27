#-----------------------
# IMPORTANTE: Los valores de los parametros de los ejercicio?_test deben ajustarse.
# Se asume que los ficheros fuente se llaman ejercicio1.c, ejercicio2.c,...,ordenar.h
#-----------------------

#No hay -ansi por: /usr/include/pcap/bpf.h:89:9: error: unknown type name ‘u_int’

CC = gcc -pedantic -g
CFLAGS = -Wall
EXE = main
RLJ = -lm -lrt
FPCAP = -lpcap

all : $(EXE) objclean

.PHONY : clean
clean :
	rm -f *.o *.txt core $(EXE)

objclean:
	@rm *.o

$(EXE) : % : %.o analiza.o hash.o list.o listControl.o
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -o $@ $@.o analiza.o hash.o list.o listControl.o $(FPCAP)

analiza.o : analiza.c analiza.h hash.h listControl.h
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $< $(FPCAP)

hash.o : hash.c hash.h list.h
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $<

list.o: list.c list.h
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(FLAGS) -c $<

listControl.o: listControl.c listControl.h
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(FLAGS) -c $<

main.o : main.c
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $<



##ordenacion.o : ordenacion.c ordenacion.h
##	@echo "#---------------------------"
##	@echo "# Generando $@"
##	@echo "# Depende de $^"
##	@echo "# Ha cambiado $<"
##	$(CC) $(CFLAGS) -c $<
##
##tiempos.o : tiempos.c tiempos.h
##	@echo "#---------------------------"
##	@echo "# Generando $@"
##	@echo "# Depende de $^"
##	@echo "# Ha cambiado $<"
##	$(CC) $(CFLAGS) $(RLJ) -c $<
	
##ejercicio1_test:
##	@echo Ejecutando ejercicio1
##	@./ejercicio1 -limInf 1 -limSup 5 -numN 10
##
##ejercicio2_test:
##	@echo Ejecutando ejercicio2
##	@./ejercicio2 -tamanio 5 -numP 5
##
##ejercicio3_test:
##	@echo Ejecutando ejercicio3
##	@./ejercicio3 -tamanio 5 -numP 5
##
##ejercicio4_test:
##	@echo Ejecutando ejercicio4
##	@./ejercicio4 -tamanio 10
##
##ejercicio5_test:
##	@echo Ejecutando ejercicio5
##	@./ejercicio5 -num_min 1 -num_max 5 -incr 1 -numP 5 -fichSalida ejercicio5.log
##
##ejercicio6_test:
##	@echo Ejecutando ejercicio6
##	@./ejercicio6 -num_min 1 -num_max 5 -incr 1 -numP 5 -fichSalida ejercicio6.log

