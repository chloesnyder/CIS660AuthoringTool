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
                EditLabel::dataHandle.brushPolish(x, y, brushRadius, brushIntensity * .05f);
            } else if (modShift) {
                EditLabel::dataHandle.brushFlatten(x, y, brushRadius, brushIntensity * .1f);
            } else {
                // if shift not pressed, then additive or subtractive based on modAlt
                EditLabel::dataHandle.brushAdd(x, y, brushRadius, brushIntensity * (modAlt ? -0.05f : 0.05f));
            }


        } else if (mode == EditMode::foliage) {
            if(modShift) {
                EditLabel::dataHandle.brushFoliageGrow(x, y, brushRadius, brushIntensity * (modAlt ? -0.05f : 0.05f));
            } else {
                EditLabel::dataHandle.brushGrassGrow(x, y, brushRadius, brushIntensity * (modAlt ? -0.05f : 0.05f));
            }
        }
        if (mode == EditMode::terrain) EditLabel::dataHandle.refreshRegion(x, y, brushRadius);
        else if (mode == EditMode::foliage) EditLabel::dataHandle.refreshRegion(x, y, brushRadius);

        refreshImage();
    }
}

QImage EditLabel::getFoliageExport()
{
    return EditLabel::dataHandle.exportFoliageImage();
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
                EditLabel::dataHandle.phDoErosion(32768);
                this->setPixmap(QPixmap::fromImage(*imgRef));
                break;
            case Qt::Key_F:
                EditLabel::dataHandle.phDoEcosystem(262144);
                if (mode == EditMode::foliage) {
                    this->setPixmap(QPixmap::fromImage(*imgRef));
                }
                break;
            case Qt::Key_K:
                EditLabel::dataHandle.phDoFires(8192);
                this->setPixmap(QPixmap::fromImage(*imgRef));
                break;
            case Qt::Key_Y:
                int years = 2;
                for (int year = 0; year < years; ++year) {
                    EditLabel::dataHandle.phDoEcosystem(131072);
                    EditLabel::dataHandle.phDoErosion(16384);
                    EditLabel::dataHandle.phDoFires(4096);
                    this->setPixmap(QPixmap::fromImage(*imgRef));
                    EditLabel::dataHandle.simRefresh(); // recalculate info like soil
                    EditLabel::dataHandle.phDoEcosystem(131072);
                    EditLabel::dataHandle.phDoErosion(16384);
                    EditLabel::dataHandle.phDoFires(4096);
                    EditLabel::dataHandle.simRefresh();
                    qDebug()<< "Finished year " << (year + 1) << " of " << years;
                }
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
        this->refreshImage();
    }
}
