#ifndef EDITLABEL_H
#define EDITLABEL_H
#pragma once
#include <QObject>
#include <QKeyEvent>
#include <imagedatamanipulator.h>

enum EditMode {terrain, surface, foliage};

class EditLabel : public QLabel
{
    Q_OBJECT
private:
    QImage *imgRef = NULL;
    bool mouseDown = false;
    int brushRadius = 10;
    bool modAlt = false; // alt held down
    bool modShift = false; // shift held down
    EditMode mode = EditMode::terrain;
public:
    EditLabel();
    EditLabel(const QString & text, QWidget * parent = 0);
    ~EditLabel() { if(imgRef) imgRef = NULL; /* wild pointer, replace later */}
    void setImageData(QImage& image);
    void refreshImage();

    static ImageDataManipulator dataHandle;
    void setEditMode(EditMode m) { mode = m; }

    void setBrushRadius(int r) { brushRadius = std::max(1, r); }
signals:
    void mouseEvent(float x, float y);
public slots:
    void mouseEdit(float x, float y);
protected:
    void mousePressEvent ( QMouseEvent * event );
    void mouseReleaseEvent ( QMouseEvent * event );
    void mouseMoveEvent ( QMouseEvent * event );
    void keyReleaseEvent (QKeyEvent *r);
    void keyPressEvent (QKeyEvent *r);
};

#endif // EDITLABEL_H
