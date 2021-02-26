#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "Functions.h"
#include "HashTable.h"

// Ler dados
List readData(FILE* file) {
  List list = mkList(mkInstr(NOTHING, empty(), empty(), empty()), NULL);

  char line[100];
  char variable1[100];
  char variable2[100];

  while(fgets(line, 100, file) != NULL) {
    if(strcmp(line, "quit\n") == 0) {
      Instr instruction = mkInstr(QUIT, empty(), empty(), empty());
      list = addLast(instruction, list);
      break;
    }
    // sscanf() lê o input formatado de uma string
    if(sscanf(line, "ler(%s)\n", variable1)) {
      // strtok() parte a string em uma serie de tokens usando um delimitador
      char* variable1 = strtok(variable1, ")");
      Instr instruction = mkInstr(READ, mkVar(variable1),empty(),empty());
      list = addLast(instruction, list);
    }
    else if(sscanf(line, "escrever(%s)\n", variable1)) {
      // strtok() parte a string em uma serie de tokens usando um delimitador
      char* variable1 = strtok(variable1, ")");
      Instr instruction = mkInstr(PRINT, mkVar(variable1), empty(), empty());
      list = addLast(instruction, list);
    }
    else if(sscanf(line, "goto %s\n", variable1)) {
      Instr instruction = mkInstr(GOTO, mkVar(variable1), empty(), empty());
      list = addLast(instruction, list);
    }
    else if(sscanf(line, "label %s\n", variable1)) {
      Instr instruction = mkInstr(LABEL, mkVar(variable1), empty(), empty());
      list = addLast(instruction, list);
    }
    else if(sscanf(line, "if %s goto %s\n", variable1, variable2)) {
      Instr instruction = mkInstr(IF_GOTO, mkVar(variable1), mkVar(variable2), empty());
      list = addLast(instruction, list);
    }
    else {
      // Caso seja uma operação matematica
      // O >6 significa que nao é uma atribuição do tipo "x = 6", porque tem 6 caracteres contando com o "\n" (mudança de linha) 
      if(strlen(line) > 6) {
          char* elements[10];
          // strtok() parte a string em uma serie de tokens usando um delimitador
          char* lineElement = strtok(line, " ");
          int index = 0;

          while(lineElement != NULL) {
              elements[index] = lineElement;
              lineElement = strtok(NULL, " ");
              index++;
          }

          Elem mainVarible = mkVar(elements[0]);
          Elem element1;
          Elem element2;
          Instr instruction7;
          // atoi() converte um valor da string para inteiro
          int intValue = atoi(elements[2]);

          if(intValue != 0) {
            element1 = mkInt(atoi(elements[2]));
          }
          else {
            element1 = mkVar(elements[2]);
          }

          intValue = atoi(elements[4]);

          if(intValue != 0) {
            element2 = mkInt(atoi(elements[4]));
          }
          else {
             lineElement = strtok(elements[4], "\n");
             element2 = mkVar(lineElement);
          }

          if(strcmp(elements[3], "+") == 0) {
            instruction = mkInstr(ADD, mainVarible, element1, element2);
          }
          else if(strcmp(elements[3], "-") == 0) {
            instruction = mkInstr(SUB, mainVarible, element1, element2);
          }
          else if(strcmp(elements[3], "*") == 0) {
            instruction = mkInstr(MUL, mainVarible, element1, element2);
          }
          else if(strcmp(elements[3], "/") == 0) {
            instruction = mkInstr(DIV, mainVarible, element1, element2);
          }

          list = addLast(instruction, list);
      }
      else {
          char* elements[3];
          // strtok() parte a string em uma serie de tokens usando um delimitador
          char* lineElement = strtok(line, " ");
          int index = 0;

          while(lineElement != NULL) {
             elements[index] = lineElement;
             lineElement = strtok(NULL, " ");
             index++;
          }

          Elem mainVarible = mkVar(elements[0]);
          Elem element;
          // atoi() converte um valor da string para inteiro
          int intValue = atoi(elements[2]);

          if(intValue != 0) {
              element = mkInt(intValue);
          }
          else {
              element = mkVar(elements[2]);
          }

          Instr instruction = mkInstr(ATRIB, mainVarible, element, empty());
          list = addLast(instruction, list);
      }
    }
  }

  return list;
}

// "Compilador"
void runData(List list, List jumpList) {
  double operationResult;
  int scan;

  while(list->nextInstruction != NULL) {
    Instr instruction = list->element;

    switch(instruction.operation) {
      case ATRIB:
        insert(instruction.firstElement.content.name, getValue(instruction.secondElement));
        break;
      case ADD:
        operationResult = getValue(instruction.secondElement) + getValue(instruction.thirdElement);
        insert(instruction.firstElement.content.name, operationResult);
        break;
      case SUB:
        operationResult = getValue(instruction.secondElement) - getValue(instruction.thirdElement);
        insert(instruction.firstElement.content.name, operationResult);
        break;
      case MUL:
        operationResult = getValue(instruction.secondElement) * getValue(instruction.thirdElement);
        insert(instruction.firstElement.content.name, operationResult);
        break;
      case DIV:
        operationResult = getValue(instruction.secondElement) / getValue(instruction.thirdElement);
        insert(instruction.firstElement.content.name, operationResult);
        break;
      case IF_GOTO:
        if(lookup(instruction.firstElement.content.name) != NULL) {
          list = jumpLine(jumpList, instruction.secondElement.content.name);
        }

        break;
      case PRINT:
        printf("%d\n", lookup(instruction.firstElement.content.name)->element);
        break;
      case READ:
        scanf("%d", &scan);
        insert(instruction.firstElement.content.name, scan);
        break;
      case GOTO:
        list = jumpLine(jumpList, instruction.firstElement.content.name);
        break;
      case LABEL:
        break;
      case QUIT:
        exit(0);
        break;
      case NOTHING:
        break;
    }

    list = tail(list);
  }
}

// Criar um elemento vazio
Elem empty() {
  Elem element;

  element.kind = EMPTY;

  return element;
}

// Criar uma lista de instruções
List mkList(Instr head, List tail) {
  // Reserva um bloco de memoria com um numero especifico de bytes
  List list = (List) malloc(sizeof(List)); 

  list->instruction = head;
  list->nextInstruction = tail;

  return list;
}

// Criar uma instrução
Instr mkInstr(OpKind operation, Elem element1, Elem element2, Elem element3) {
  Instr instruction;

  instruction.operation = operation;
  instruction.firstElement = element1;
  instruction.secondElement = element2;
  instruction.thirdElement = element3;

  return instruction;
}

// Criar uma variavel
Elem mkVar(char* string) {
  Elem element;

  element.kind = STRING;
  element.content.name = string;

  return element;
}

// Criar um inteiro
Elem mkInt(int n) {
  Elem element;

  element.kind = INT_CONST;
  element.content.value = n;

  return element;
}

// Adiciona uma instrução ao fim da lista
List addLast(Instr instruction, List list) {
  if(list == NULL) {
    return mkList(instruction, NULL);
  }

  return mkList(list->instruction, addLast(instruction, tail(list)));
}

// Retorna a "cauda" da lista
List tail(List l) {
  if(l == NULL) {
    return NULL;
  }

  return l->nextInstruction;
}

// Retorna o valor do element
int getValue(Elem element) {
  int elementValue;

  if(element.kind == STRING) {
    if(lookup(element.content.name) != NULL) {
      elementValue = lookup(element.content.name)->element;

      return elementValue;
    }
  }
  else if(element.kind == INT_CONST) {
    elementValue = element.content.value;

    return elementValue;
  }
  else {
    printf("ERROR: element does not exist!!\n");
    exit(0);
  }

  return 0;
}

// Salta para outra linha
List jumpLine(List list, char* elementName) {
  while(list != NULL) {
    if(list->instruction.operation == LABEL && !strcmp(list->instruction.first.content.name, elementName)) {
      return list;
    }

    list = list->nextInstruction;
  }

  printf("ERROR: label %s does not exist!!\n", elementName);
  exit(EXIT_FAILURE);

  return NULL;
}