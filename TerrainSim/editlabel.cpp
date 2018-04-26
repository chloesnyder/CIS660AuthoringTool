#include "editlabel.h"

ImageDataManipulator EditLabel::dataHandle;



EditLabel::EditLabel() : QLabel()
{

    connect( this, SIGNAL( mouseEvent(float,float) ), this, SLOT( mouseEdit(float,float) ) );
}

EditLabel::EditLabel( const QString & text, QWidget * parent) : QLabel(parent)
{

    connect( this, SIGNAL( mouseEvent(float,float) ), this, SLOT( mouseEdit(float,float) ) );
}

void EditLabel::refreshImage()
{
    this->setPixmap(QPixmap::fromImage(*imgRef));
}

void EditLabel::mouseEdit(float x, float y)
{
    if (imgRef) {
        if (mode == EditMode::terrain) {
            if(modAlt && modShift)
            {
                EditLabel::dataHandle.brushPolish(x, y, brushRadius, .02);
            } else if (modShift) {
                EditLabel::dataHandle.brushFlatten(x, y, brushRadius, .02); // may need to change amt
            } else {
                // if shift not pressed, then additive or subtractive based on modAlt
                EditLabel::dataHandle.brushAdd(x, y, brushRadius, (modAlt ? -0.01 : 0.01));
            }


        } else if (mode == EditMode::foliage) {

        }
        EditLabel::dataHandle.refreshRegion(x, y, brushRadius);
        refreshImage();
    }
}

void EditLabel::mousePressEvent(QMouseEvent *event)
{
    emit mouseEvent((float)event->x(), (float)event->y());
    mouseDown = true;
    setFocus();
}

void EditLabel::mouseReleaseEvent(QMouseEvent *event)
{
    mouseDown = false;
}

void EditLabel::mouseMoveEvent(QMouseEvent *event)
{
    if (mouseDown) {
        emit mouseEvent((float)event->x(), (float)event->y());
    }
}

void EditLabel::keyPressEvent(QKeyEvent *r)
{
    switch(r->key()) {
        case Qt::Key_Alt:
            modAlt = true;
            break;
        case Qt::Key_Shift:
            modShift = true;
            break;
    }
}

void EditLabel::keyReleaseEvent(QKeyEvent *r)
{
    if (r->isAutoRepeat()) {
        r->ignore();
    } else {
        switch(r->key()) {
            case Qt::Key_Alt:
                modAlt = false;
                break;
            case Qt::Key_Shift:
                modShift = false;
                break;
            case Qt::Key_E:
                EditLabel::dataHandle.phDoErosion(20000);
                this->setPixmap(QPixmap::fromImage(*imgRef));
                break;
        }
    }
}

void EditLabel::setImageData(QImage &image)
{
    if (mode == EditMode::terrain) {
        EditLabel::dataHandle = ImageDataManipulator(image);
        imgRef = &image;
    } else if (mode == EditMode::foliage) {
        EditLabel::dataHandle.setFoliageRef(image);
        imgRef = &image;
    }
}
