# Setup

Before installing this be sure that you have read the <a href="https://github.com/albertodev01/TEquations/blob/master/C%2B%2B/Qt/README.md">README.md</a> in the previous folder. Let's start:

 1. Download the plugin source files that you can find here and place them in a folder called `EquationPlot` (or whatever!)
 2. Donwload the library source files and place them in the `EquationPlot` folder. It should look like this:
 
 <p align="center"><img src="https://github.com/albertodev01/TEquations/blob/master/C%2B%2B/Qt/github_images/dir.png" /></p>
 
 3. Place this folder somewhere (preferably in the same folder as your main project) and you are ready to go!
 
 #Usage example
 
 Let's do a quick example.
 
  1. Create a new widget application (I use MSVC 2017)
  2. Right click on the project name on the left > Add Existing Files > go to the `EquationPlot` folder and import all the files. You should have something like this:
  
   <p align="center"><img src="https://github.com/albertodev01/TEquations/blob/master/C%2B%2B/Qt/github_images/proj.png" /></p>
   
  3. Now open the `.ui` file > drag and drop a QWidget plugin > right click on it > Promote to... > Set the "Promoted class name" to `EquationPlot` > Put in "Header file" the path to `equationplot.h` > Click Add > Click promote
  4. Save all, build and run. This will be the result!
  
  <p align="center"><img src="https://github.com/albertodev01/TEquations/blob/master/C%2B%2B/Qt/github_images/res0.png" /></p>

  5. Let's try to solve `f(x) = x^2-4.2`. Call the widger `Solver`. Drop a button below the widget and set a `clicked` slot. Add this code:
  
```c++
void MainWindow::on_pushButton_clicked() {
    //display dots
    ui->Solver->setDots(true);
    //can zoom in/zoom out with the mouse wheel
    ui->Solver->setWheel(true);
    //set the equation to be solved. If you want to know why the QVector has those number is, read the doc in the Source folder
    ui->Solver->setEquation("x^2-4.2", EquationPlot::SolverType::Newton, QVector<double>{2, 1.0e-10, 20}, true);

    //structured bindings (C++17 feature)
    //the solveEquation() returns a tuple. See the doc in the Source folder
    auto [x0, res, sol] = ui->Solver->solveEquation();

    QString temp{ "x0 = " + QString::number(x0, 'f', 10) + "\nf(x0) = " + QString::number(res, 'e', 10) + "\n\n" };
    for(const auto& i : sol)
        temp += QString::number(i, 'f', 15) + "\n";

    QMessageBox::information(this, "Result", temp);
}
```

Done! Now build your app, run it and click the button to see the the solutions and the chart!

<p align="center"><img src="https://github.com/albertodev01/TEquations/blob/master/C%2B%2B/Qt/github_images/res1.png" /></p>

Since you have set `ui->Solver->setDots(true);` if you try to move the wheel of the mouse up or down, you will be able to zoom in and out the chart.

<p align="center"><img src="https://github.com/albertodev01/TEquations/blob/master/C%2B%2B/Qt/github_images/res2.gif" /></p>

In the same way, you can solve polynomials of 2nd, 3rd and 4th degree using `setPolynomial()` and then `solvePolynomial()`.
