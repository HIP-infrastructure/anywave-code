import setuptools

with open("README.md", "r") as fh:
    long_description = fh.read()
	
setuptools.setup(
	name = "anywave package",
	version = "1.0",
	author = "Bruno Colombet",
	author_email = "bruno.colombet@univ-amu.fr",
	long_description = long_description,
	long_description_content_type="text/markdown",
	url="https://meg.univ-amu.fr/wiki/AnyWave",
	packages=setuptools.find_packages(),
	install_requires=['numpy', 'PyQt5'],
	classifiers = [
		"Programming Language :: Pyhton :: 3",
		"License :: OSI Approved :: lGPL3",
		"Operating System :: OS Independent",
	],
	python_requires = '>=3.6',
)