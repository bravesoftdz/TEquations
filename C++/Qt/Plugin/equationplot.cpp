#include "equationplot.h"

void EquationPlot::setup() {
    xmax = 5;
    xmin = -5;
    ymax = 5;
    ymin = -5;

    hasBorder = true;
    hasPoints = solved = canWheel = false;

    setMinimumSize(QSize{300,300});
}

void EquationPlot::setBorder(bool value) {
    hasBorder = value;
    repaint();
}

void EquationPlot::setDots(bool value) {
    hasPoints = value;
    repaint();
}

void EquationPlot::setKind(EquationType kind) {
    type = kind;
}

void EquationPlot::ZoomIn() {
    if (xmax > 1) {
        xmax -= 1;
        ymax -= 1;
        xmin += 1;
        ymin += 1;
        repaint();
    }
}

void EquationPlot::ZoomOut() {
    if (xmax < 21) {
        xmax += 1;
        ymax += 1;
        xmin -= 1;
        ymin -= 1;
        repaint();
    }
}

void EquationPlot::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.eraseRect(0, 0, width(), height());
    painter.fillRect(0, 0, width(), height(), Qt::white);

    auto pw = static_cast<double>(width());
    auto ph = static_cast<double>(height());

    double distX = (pw / 2) / xmax;
    double distY = (ph / 2) / ymax;

    painter.setPen(QPen(Qt::gray));
    auto prevPoint = QPointF{distX, 0};
    auto currPoint = QPointF{0, distY};

    //Draw the grid
    for(auto i = xmin; i <= xmax; ++i) {
        if (i == 0)
            continue;

        painter.drawLine( prevPoint, QPointF{prevPoint.x(), ph} );
        painter.drawLine( currPoint, QPointF{pw, currPoint.y()} );
        prevPoint.setX(prevPoint.x() + distX);
        currPoint.setY(currPoint.y() + distY);
    }

    //Draw X and Y axes
    painter.setPen(QPen{Qt::black, 2});
    painter.drawLine(QPointF{0, ph/2}, QPointF{pw, ph/2});
    painter.drawLine(QPointF{pw/2, 0}, QPointF{pw/2, ph});

    if (hasBorder) {
        painter.drawLine(QPointF{0, 0}, QPointF{0, ph});
        painter.drawLine(QPointF{pw, 0}, QPointF{pw, ph});
        painter.drawLine(QPointF{0, ph}, QPointF{pw, ph});
        painter.drawLine(QPointF{0, 0}, QPointF{pw, 0});
    }

    if (hasPoints) {
        painter.setPen(QPen{Qt::blue, 1});
        auto midPointS = QPointF{-3, (ph / 2) - 3};
        auto midPointC = QPointF{(pw / 2) - 3, -3};

        for(auto j = xmin; j <= (xmax + 1); ++j) {
            if (j == 0)
                continue;

            painter.fillRect(QRectF{midPointS.x(), midPointS.y(), 6, 6}, Qt::blue);
            painter.fillRect(QRectF{midPointC.x(), midPointC.y(), 6, 6}, Qt::blue);
            midPointS.setX(midPointS.x() + distX);
            midPointC.setY(midPointC.y() + distY);
        }
    }

    painter.setPen(QPen{QColor{249, 156, 74}, 1});

    if (solved) {
        auto prevPoint = QPointF{0, 0};
        double logy = 0;
        std::unique_ptr<PolyBase> poly;
        std::unique_ptr<Equation> equation;

        if (type == etEquation) {
            equation = std::make_unique<Equation>(eqOpt.input);
        } else {
            switch (polyOpt.poltype) {
            case pQuadratic:
                poly = std::make_unique<Quadratic>(polyOpt.values[0], polyOpt.values[1], polyOpt.values[2]);
                break;
            case pCubic:
                poly = std::make_unique<Cubic>(polyOpt.values[0], polyOpt.values[1], polyOpt.values[2], polyOpt.values[3]);
                break;
            case pQuartic:
                poly = std::make_unique<Quartic>(polyOpt.values[0], polyOpt.values[1], polyOpt.values[2], polyOpt.values[4], polyOpt.values[5]);
                break;
            }
        }

        for(auto i = 0; i < width(); ++i) {
            auto logx = ScreenToLog(QPointF(i, 0)).x();

            if (type == etEquation)
                logy = equation->evaluateOn(logx);
            else {
                switch (polyOpt.poltype) {
                case pQuadratic:
                    logy = dynamic_cast<Quadratic*>(poly.get())->evaluateOnX(logx);
                    break;
                case pCubic:
                    logy = dynamic_cast<Cubic*>(poly.get())->evaluateOnX(logx);
                    break;
                case pQuartic:
                    logy = dynamic_cast<Quartic*>(poly.get())->evaluateOnX(logx);
                    break;
                }
            }

            auto points = QPointF{logx, logy};
            auto currPoint = QPointF{static_cast<double>(i), LogToScreen(points).y()};

            if ((currPoint.y() >= 0) && (currPoint.y() <= height()))
                painter.drawLine(currPoint, prevPoint);
            prevPoint = currPoint;
        }
    }

    event->accept();
}

void EquationPlot::setWheel(bool value) {
    canWheel = value;
}

void EquationPlot::wheelEvent(QWheelEvent* event) {
    if (canWheel) {
        int d = event->angleDelta().y() / 8;
        if (auto steps = d / 15; steps >= 0)
            ZoomIn();
        else
            ZoomOut();
        event->accept();
    }
}

QPointF EquationPlot::ScreenToLog(QPointF ScreenPoint) const {
    return QPointF{ xmin + (ScreenPoint.x() / width()) * (xmax - xmin),
                    ymin + (ymax - ymin) * (height() - ScreenPoint.y()) / height()};
}

QPointF EquationPlot::LogToScreen(QPointF LogPoint) const {
    return QPointF{ width() * (LogPoint.x() - xmin) / (xmax - xmin),
                    height() - (height() * (LogPoint.y() - ymin) / (ymax - ymin))};
}

void EquationPlot::setEquation(const QString& equation, SolverType etype, const QVector<double>& points, bool GuessList) {
    this->type = etEquation;
    eqSolver = etype;

    eqOpt.input = equation.toStdString();
    eqOpt.points = points.toStdVector();
    eqOpt.hasGuesses = GuessList;
}

Result EquationPlot::solveEquation() {
    Equatio = std::make_unique<Equation>(eqOpt.input);
    Result tmp;

    switch (eqSolver) {
    case Newton:
        tmp = Equatio->solveEquation(Algorithm::Newton, eqOpt.points, eqOpt.hasGuesses);
        break;
    case Secant:
        tmp = Equatio->solveEquation(Algorithm::Secant, eqOpt.points, eqOpt.hasGuesses);
        break;
    }

    solved = true;
    repaint();

    return tmp;
}

void EquationPlot::setPolynomial(const QVector<double>& points) {
    this->type = etPolynomial;

    switch (points.size()) {
    case 3:
        polyOpt.poltype = pQuadratic;
        break;
    case 4:
        polyOpt.poltype = pCubic;
        break;
    case 5:
        polyOpt.poltype = pQuartic;
        break;
    }

    polyOpt.values = points.toStdVector();
}

PolySolutions EquationPlot::solvePolynomial() {
    PolySolutions sol;

    switch (polyOpt.poltype) {
    case pQuadratic:
        Polynom = std::make_unique<Quadratic>(polyOpt.values[0], polyOpt.values[1], polyOpt.values[2]);
        sol.discriminant = dynamic_cast<Quadratic*>(Polynom.get())->getDiscriminant();
        break;
    case pCubic:
        Polynom = std::make_unique<Cubic>(polyOpt.values[0], polyOpt.values[1], polyOpt.values[2], polyOpt.values[3]);
        sol.discriminant = dynamic_cast<Cubic*>(Polynom.get())->getDiscriminant();
        break;
    case pQuartic:
        Polynom = std::make_unique<Quartic>(polyOpt.values[0], polyOpt.values[1], polyOpt.values[2], polyOpt.values[3], polyOpt.values[4]);
        sol.discriminant = dynamic_cast<Quartic*>(Polynom.get())->getDiscriminant();
        break;
    }

    sol.solutions = Polynom->getSolutions();

    solved = true;
    repaint();

    return sol;
}
