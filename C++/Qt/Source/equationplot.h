#ifndef EQUATIONPLOT_H
#define EQUATIONPLOT_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QPointF>
#include <QVector>
#include <string>
#include <vector>
#include <memory>
#include <QPaintEvent>
#include <QWheelEvent>
#include "Equation.h"

using namespace NA_Equation;

class PolySolutions {
friend class EquationPlot;
private:
    double discriminant;
    PolyResult solutions;
public:
    PolySolutions() = default;
    double getDiscriminant() const { return discriminant; }
    PolyResult getSolutions() const { return solutions; }
};

class EquationPlot : public QWidget {

    Q_OBJECT

public:
    enum EquationType { etEquation = 0, etPolynomial };
    Q_ENUM(EquationType)

    enum PolyType { pQuadratic = 0, pCubic, pQuartic };
    Q_ENUM(PolyType)

    enum SolverType { Newton = 0, Secant };
    Q_ENUM(SolverType)

private:
    //screen vars
    bool hasBorder;
    bool hasPoints;
    bool solved;
    bool canWheel;
    int xmin, xmax, ymin, ymax;
    //library vars
    EquationType type;
    SolverType eqSolver;
    std::unique_ptr<PolyBase> Polynom;
    std::unique_ptr<Equation> Equatio;
    //methods
    void setup();
    QPointF ScreenToLog(QPointF ScreenPoint) const;
    QPointF LogToScreen(QPointF LogPoint) const;

    struct EquationOptions {
        std::string input;
        std::vector<double> points;
        bool hasGuesses;
    };
    EquationOptions eqOpt;

    struct PolynomialOptions {
        PolyType poltype;
        std::vector<double> values;
    };
    PolynomialOptions polyOpt;

protected:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

public:
    explicit EquationPlot(QWidget *parent = nullptr): QWidget{parent}, type{etEquation}, eqSolver{Newton} { setup(); }
    ~EquationPlot() override = default;

    //setters
    void setBorder(bool value);
    void setDots(bool value);
    void setKind(EquationType kind);
    void setWheel(bool value);    
    void setEquation(const QString& equation, SolverType etype, const QVector<double>& points, bool GuessList);
    void setPolynomial(const QVector<double>& points);

    //getters
    bool getBorder() const { return hasBorder; }
    bool getDots() const { return hasPoints; }

    //methods
    Result solveEquation();
    PolySolutions solvePolynomial();
    void ZoomIn();
    void ZoomOut();
};

#endif // QEQUATIONPLOT_H
