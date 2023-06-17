import numpy as np
import pandas as pd

class TermTableConverter:
    def __init__(self, csv_file):
        self.csv_file = csv_file
        self.data = None
        self.table = None
        self.data_array = None
        self.term_table = None

    def read_csv(self):
        self.data = np.loadtxt(self.csv_file, delimiter=',')

    def generate_term_table(self):
        self.table = {}
        for microStateConfig in self.data:
            column = microStateConfig[1]
            value = microStateConfig[0]

            if column not in self.table:
                self.table[column] = []
            self.table[column].append(value)

    def visualize_term_table(self):
        columns_ordered = sorted([key for key in self.table.keys()])
        index = sorted(set(val for sublist in self.table.values() for val in sublist))
        self.term_table = pd.DataFrame(index=index, columns=columns_ordered)
        self.term_table.fillna(0, inplace=True)

        for key, values in self.table.items():
            for value in values:
                self.term_table.loc[value, key] += 1

    def convert_array(self):
        self.data_array = np.array(self.data)

    def process_csv(self):
        self.read_csv()
        self.generate_term_table()
        self.visualize_term_table()

    def display_term_table(self):
        print(self.term_table)


def main(csv_name):
    converter = TermTableConverter(csv_name)
    converter.process_csv()
    converter.display_term_table()


if __name__ == "__main__":
    pathToCsv = "output.csv"
    main(pathToCsv)