import csv
import sys
import pandas as pd

def exit_help():
    print(' lab1.py (mealy-to-moore|moore-to-mealy) inputFile outputFile ')
    sys.exit(0)

def MiliToMur():
  dictionary = {}
  inputSignals = []
  for element in array:
      if array[0] != element:
        for el in element:
          if element[0] != el:
            dictionary[el] = el.split(sep="/")
            dictionary[el].append(element.index(el))
          else:
            inputSignals.append(el)
  indexOfDict = list(dictionary.keys())  
  m = len(dictionary) + 1
  n = len(inputSignals) + 2
  resultArray = [[""] * m for i in range(n)]

  resultArray[0][0] = ""
  number = 2
  for i in inputSignals:
       resultArray[number][0] = i
       number += 1
  number = 1

  for key in dictionary:
     resultArray[0][number] = dictionary[key][1]
     resultArray[1][number] = number
     for i in range(len(inputSignals)):
       a = dictionary[key][0]
       b = array[0].index(a)
       d = indexOfDict.index(array[i+1][b])+1
       resultArray[i+2][number] = d
     number += 1
  return resultArray

def MurToMili():
  ar = array[0]
  m = len(ar)
  n = len(array)
  resultArray = [[""] * m for i in range(n-1)]
  number = 1
  while number < n:
    i = 0
    while i < m:
       if(i == 0 or number == 1):
         resultArray[number-1][i] = array[number][i]
       else:
         resultArray[number-1][i] = array[number][i] + "/" + ar[i]
       i += 1
    number += 1
  return resultArray

if __name__ == '__main__':
    args = sys.argv[1:]

    if len(args) != 3:
        exit_help()

    inputFile = args[1]
    with open(inputFile, encoding='utf-8') as r_file:
      file_reader = csv.reader(r_file, delimiter = ";")
      count = 0
      array = []
      for row in file_reader:
        str1 = ";".join(row)
        str1.split(sep=";") 
        array.append(str1.split(sep=";"))
        count += 1
    
    resultArray = []
    if args[0] == 'mealy-to-moore':
        resultArray = MiliToMur()
        print('OK mealy')
    elif args[0] == 'moore-to-mealy':
        resultArray = MurToMili()
        print('OK moore')
    else:
        exit_help()

    outputFile = args[2]
    result = pd.DataFrame(resultArray)
    print(result)
    with open(outputFile, 'w', newline='\n') as f2:
      csv.writer(f2, delimiter=';').writerows(resultArray)