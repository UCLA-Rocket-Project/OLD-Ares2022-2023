import pandas as pd

#csv file name to be read in 
in_csv = 'adc_raw.csv'

#get the number of lines of the csv file to be read
number_lines = sum(1 for row in (open(in_csv)))

#size of rows of data to write to the csv, 
#you can change the row size according to your need
rowsize = 1030000

#start looping through data writing it to a new file for each set
print(number_lines)
print(rowsize)
print(int(number_lines/rowsize))
for i in range(0,int(number_lines/rowsize)+1): # +1 for rounding up
    print(i)
    j = i * rowsize
    print(j)
    df = pd.read_csv(in_csv,
          header=None,
          nrows = rowsize,#number of rows to read at each loop
          skiprows=j)#skip rows that have been read

    #csv to write data to a new file with indexed name. input_1.csv etc.
    out_csv = 'adc_raw' + str(i) + '.csv'
    df.to_csv(out_csv,
          index=False,
          header=False,
          mode='a',#append data to csv file
          chunksize=rowsize)#size of data to append for each loop