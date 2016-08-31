
1. The documentation for the particulate only version of the sensor can be
   found in the "particulate_only" sub-directory.

2. The documentation for the gas and particulate version of the sensor can be
   found in the "gas_and_particulate" sub-directory.

3. The documentation is created using Sphinx documentation tool. 
   http://www.sphinx-doc.org/en/stable/index.html 

4. A reasonably recent version of sphinx (>= v1.2.2) is required to build the
   html or pdf  versions of the documentation from the restructured text (.rst)
   source files. Installation instructions for Sphinx can be found here 
   http://www.sphinx-doc.org/en/stable/install.html

5. To build the html version of the documentation run "make html" from inside
   the documentation sub-directory - i.e. either the "particulate_only" or
   "gas_and_particulate".  The documentation will be created in a sub-directory
   named "_build/html".

6. To build the pdf version of the documentation a working version of latex is
   required https://www.latex-project.org/.  First run "make latex" in the 
   documentation sub-directory - i.e. either "particulate_only" or
   "gas_and_particulate". A latex verion of the documentation will be
   created in the "_build/latex" sub-directory. A pdf can then be generated
   by running "make" in the "_build/latex" sub-directory.

7. To clean the project directories (if desired) run "make clean".   Note,
   this will remove all generated documenation and intermediary files.



 





