#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H
#pragma once
#include <QMainWindow>
#include <QImage>
#include <QHBoxLayout>
#include <qtabwidget.h>
#include <imagedatamanipulator.h>
#ifndef QT_NO_PRINTER
#include <QPrinter>
#endif
#include <editlabel.h>

class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
class QSlider;

class ImageViewer : public QMainWindow
{
    Q_OBJECT

public:
    ImageViewer();
    bool loadFile(const QString &);
    void refreshImage();

private slots:
    void open();
    void saveAs();
    void copy();
    void paste();
    void zoomIn();
    void zoomOut();
    void normalSize();
    void fitToWindow();
    void about();

private:


    void createActions();
    void createMenus();
    void updateActions();
    bool saveFile(const QString &fileName);
    void setImage(const QImage &newImage);
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);
    QTabWidget* imageTabs;
    QWidget* central;
    QHBoxLayout* layout;
    QImage image;
    QImage foliage;
    EditLabel *heightLabel;
    EditLabel *foliageLabel;
    QScrollArea *scrollArea;
    double scaleFactor;

    QSlider *intensitySlider;

#ifndef QT_NO_PRINTER
    QPrinter printer;
#endif

    QAction *saveAsAct;
    QAction *copyAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;
};

#endif
