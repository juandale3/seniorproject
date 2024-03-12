import os
import pandas as pd

'''
Using this function make sure it opens up the data folder within our program, 
Write code for file explorer to open, make sure it opens in the write folder,

need to add initData.txt into data folder


'''




def openFileExplorer():
    os.system('explorer ".\data"')

test_array = [1,2,3,4,5]
coloumn = []
def pumpData(pump_array_data,columns):

    new_data_df = pd.DataFrame(pump_array_data, columns)

    # Load the existing Excel file into a DataFrame
    file_path = 'example.xlsx'
    original_df = pd.read_excel(file_path)

    # Append the new data
    updated_df = original_df.append(new_data_df, ignore_index=True)

    # Write the updated DataFrame to the Excel file
    # Using ExcelWriter and openpyxl to preserve existing sheets/formatting if necessary
    with pd.ExcelWriter(file_path, engine='openpyxl', mode='a', if_sheet_exists='replace') as writer:
        # Assuming your data is in the first sheet, you can replace it with the updated DataFrame
        updated_df.to_excel(writer, sheet_name='Sheet1', index=False)

