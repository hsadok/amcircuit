from subprocess import call
from matplotlib.backends.backend_pdf import PdfPages
import pandas as pd
from matplotlib import pyplot as PLT

netlists_dir = "support/"
expected_dir = "support/expected_data/"
result_dir = "support/result_data/"
program_exec_path = "../bin/amcircuit_main"
data_file_names = ["artefato", "ch5", "diode", "el5", "estabilidade", "mres", "rc", "rl", "sc", "tesla"]

with PdfPages(result_dir + 'diff.pdf') as pdf:
    for file_name in data_file_names:
        print file_name
        call([program_exec_path, netlists_dir + file_name + ".net", result_dir + file_name + ".tab"])
        expected = pd.read_csv(expected_dir + file_name + ".tab", sep='\s+', index_col=0)
        result = pd.read_csv(result_dir + file_name + ".tab", sep='\s+', index_col=0)
        join = expected-result
        try:
            join.plot(title=file_name + " error")
        except OverflowError, e:
            print "Overflow! skipping..."
        pdf.savefig()  # saves the current figure into a pdf page
        PLT.close()
    

#PLT.show() # used when not saving to pdf
