<div align="center">
  <img src="./images/tubiao.ico" alt="Project Icon" width="150" height="150">
  <h2><strong>A simple Software for Finite Element Method</strong></h2>
</div>

**Archive**: I built this repo as a toy finite element calculation gui in my undergraduate. However, it is not maintained now. So if you have any question with this repo, please email me.


**ElementMaster** is a finite element analysis software that allows users to model, mesh partition, compute, and generate stress-strain contour plots.

![Interface Screenshot](./images/userinterface.png)


## Directory Structure
- `images`: This folder contains images used in the project, such as screenshots or icons.

- `qt`: This folder contains the QT code files, this is where you can find the implementation of the software's functionalities.

- `README.md`: This is the README file that provides information about the project, including the directory structure and usage instructions.

## Functionality:

- :eyes:Model: You can add models by drawing with the mouse or manually setting the coordinates. After modeling, please remember to add Poisson's ratio and Elasticity modulus of the material. 

- :sweat_drops:Mesh Partition: You can mesh through clicking the "网格" button after modeling successfully.

- :boom:Compute: Perform computations by clicking "计算" button.

- :raised_back_of_hand:Generate Stress-Strain Contour Plot: Generate a stress-strain contour plot by clicking the related button.

- :hankey: Deletion of Existing Models: After the calculations are complete, you have the option to delete the existing models.
- :robot: User Log: The software keeps a log to record user operations and actions.

Please note that the the project is develped in **Qt Creator 9.0.2**.

