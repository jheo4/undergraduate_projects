# Setuptools package is the prerequisite of 4men project
from setuptools import setup, find_packages


def get_requirements(filename='requirements.txt'):
    requirements = []
    with open(filename, 'r') as rf:
        for requirement in rf.readlines():
            if requirement.strip():
                requirements.append(requirement)
    return requirements


setup(
    name="fourmen",
    # author
    # url
    packages=find_packages(exclude=['tests']),
    install_requires=get_requirements()
)
