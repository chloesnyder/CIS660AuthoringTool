#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrintDialog>
#endif
#endif

#include "imageviewer.h"

ImageViewer::ImageViewer()
   : heightLabel(new EditLabel)
   , foliageLabel(new EditLabel)
   , scrollArea(new QScrollArea)
   , scaleFactor(1)
{
    heightLabel->setBackgroundRole(QPalette::Base);
    heightLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    heightLabel->setScaledContents(true);
    heightLabel->setMouseTracking(true);
    heightLabel->setAlignment(Qt::AlignCenter);
    heightLabel->setEditMode(EditMode::terrain);
    heightLabel->setFocusPolicy(Qt::StrongFocus);

    foliageLabel->setBackgroundRole(QPalette::Base);
    foliageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    foliageLabel->setScaledContents(true);
    foliageLabel->setMouseTracking(true);
    foliageLabel->setAlignment(Qt::AlignCenter);
    foliageLabel->setEditMode(EditMode::foliage);
    foliageLabel->setFocusPolicy(Qt::StrongFocus);

    dummy1 = new QWidget();
    layout1 = new QGridLayout();
    dummy1->setLayout(layout1);
    layout1->addWidget(heightLabel);

    dummy2 = new QWidget();
    layout2 = new QGridLayout();
    dummy2->setLayout(layout2);
    layout2->addWidget(foliageLabel);

    imageTabs = new QTabWidget();
    imageTabs->addTab(dummy1, tr("Height Editor"));
    imageTabs->addTab(dummy2, tr("Foliage Editor"));

    central = new QWidget();
    layout = new QHBoxLayout(central);
    layout->addWidget(imageTabs);

    intensitySlider = new QSlider(Qt::Vertical); // central
    intensitySlider->setFocusPolicy(Qt::StrongFocus);
    intensitySlider->setTickPosition(QSlider::TicksBothSides);
    intensitySlider->setTickInterval(25); // min value = .01, max value = .1
    intensitySlider->setSingleStep(1);
    intensitySlider->setMinimum(0);
    intensitySlider->setMaximum(100);
    intensitySlider->setValue(25);
    intensitySlider->adjustSize();

    layout->addWidget(intensitySlider);

    sizeSlider = new QSlider(Qt::Vertical); // central
    sizeSlider->setFocusPolicy(Qt::StrongFocus);
    sizeSlider->setTickPosition(QSlider::TicksBothSides);
    sizeSlider->setTickInterval(5); // min value = .01, max value = .1
    sizeSlider->setSingleStep(1);
    sizeSlider->setMinimum(0);
    sizeSlider->setMaximum(30);
    sizeSlider->setValue(10);
    sizeSlider->adjustSize();

    layout->addWidget(sizeSlider);
    connect(sizeSlider, SIGNAL(valueChanged(int)), this, SLOT(setRadius(int)));
    connect(intensitySlider, SIGNAL(valueChanged(int)), this, SLOT(setIntensity(int)));

    scrollArea->setBackgroundRole(QPalette::Light);
    scrollArea->setWidget(central);
    scrollArea->setVisible(false);
    setCentralWidget(scrollArea);

    createActions();

    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
}

void ImageViewer::setRadius(int value)
{
    value = std::max(1, value);
    foliageLabel->setBrushRadius(value);
    heightLabel->setBrushRadius(value);
}

void ImageViewer::setIntensity(int value)
{
    float v = (float) value / 100.0f;
    foliageLabel->setBrushIntensity(v);
    heightLabel->setBrushIntensity(v);
}

bool ImageViewer::loadFile(const QString &fileName)
{
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    QImage newImage = reader.read();
    qDebug()<< newImage.format();
    newImage = newImage.convertToFormat(QImage::Format_RGB32);
    qDebug()<< newImage.format();
    if (newImage.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
        return false;
    }

    setImage(newImage);

    setWindowFilePath(fileName);

    const QString message = tr("Opened \"%1\", %2x%3, Depth: %4")
        .arg(QDir::toNativeSeparators(fileName)).arg(image.width()).arg(image.height()).arg(image.depth());
    statusBar()->showMessage(message);
    return true;
}

void ImageViewer::refreshImage()
{
    heightLabel->setPixmap(QPixmap::fromImage(image));
    heightLabel->setFixedSize(image.size());
    foliageLabel->setFixedSize(image.size());
}

// reduce to nearest power of 2
uint32_t flp2 (uint32_t x)
{
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >> 16);
    return x - (x >> 1);
}

void ImageViewer::setImage(const QImage &newImage)
{
    // guarantee square image for morton encoding
    int w = std::min(newImage.size().width(), newImage.size().height());
    w = flp2(w + 1);
    QRect rect(0, 0, w, w);
    image = newImage.copy(rect);
    foliage = newImage.copy(rect);
    foliage.convertToFormat(QImage::Format_RGB32);
    heightLabel->setPixmap(QPixmap::fromImage(image));
    foliageLabel->setPixmap(QPixmap::fromImage(foliage));
    scaleFactor = 1.0;

    scrollArea->setVisible(true);
    fitToWindowAct->setEnabled(true);
    updateActions();

    if (!fitToWindowAct->isChecked()){
        heightLabel->adjustSize();
        foliageLabel->adjustSize();
    }

    int hMargin = 256;//std::min(image.size().width() * 2 / 3, 256);
    int vMargin = 128;//std::min(image.size().height() * 2 / 3, 256);
    //resize(image.size().width() + hMargin, image.size().height() + vMargin);
    central->setGeometry(0, 0, image.size().width() + hMargin, image.size().height() + vMargin);

    resize(image.size().width() + hMargin + vMargin, image.size().height() + hMargin);
    scrollArea->setGeometry(0, 0, image.size().width() + hMargin * vMargin, image.size().height() + hMargin);
    scrollArea->setAlignment(Qt::AlignCenter);
    //heightLabel->move(imageTabs->rect().center() - heightLabel->rect().center());
    foliageLabel->setAlignment(Qt::AlignCenter);
    heightLabel->setAlignment(Qt::AlignCenter);
    //foliageLabel->move(imageTabs->rect().center() - foliageLabel->rect().center());

    heightLabel->setImageData(image);
    foliageLabel->setImageData(foliage);

    this->refreshImage();
}


bool ImageViewer::saveFile(const QString &fileName)
{
    QStringList l = fileName.split('.');
    QImageWriter writer(QString(l.at(0)).append("_height.").append(QString(l.at(1))));
    writer.setFormat("bmp");
    writer.setCompression(0);

    if (!writer.write(image)) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot write %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName)), writer.errorString());
        return false;
    }
    const QString message = tr("Wrote \"%1\"").arg(QDir::toNativeSeparators(fileName));
    statusBar()->showMessage(message);

    QImageWriter writer2(QString(l.at(0)).append("_foliage.").append(QString(l.at(1))));
    writer2.setFormat("bmp");
    writer2.setCompression(0);

    QImage imgTest = foliageLabel->getFoliageExport(); // slow
    //imgTest.fill((255 << 24) + 5);

    if (!writer2.write(imgTest)) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot write %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName)), writer2.errorString());
        return false;
    }

    //QString ffname = l.at(0).append("_foliage").append(l.at(1));
    return true;
}


static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;

    if (firstDialog) {
        firstDialog = false;
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    }

    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
        ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
    foreach (const QByteArray &mimeTypeName, supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    //dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.setNameFilter("Windows BMP Image (*.bmp *.dib)");
    dialog.setDefaultSuffix(".bmp");
    //dialog.selectMimeTypeFilter("image/bmp");
    if (acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("bmp");
}

void ImageViewer::open()
{
    QFileDialog dialog(this, tr("Open File"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {}
}

void ImageViewer::saveAs()
{
    QFileDialog dialog(this, tr("Save File As"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptSave);

    while (dialog.exec() == QDialog::Accepted && !saveFile(dialog.selectedFiles().first())) {}
}

void ImageViewer::copy()
{
#ifndef QT_NO_CLIPBOARD
    QGuiApplication::clipboard()->setImage(image);
#endif // !QT_NO_CLIPBOARD
}

#ifndef QT_NO_CLIPBOARD
static QImage clipboardImage()
{
    if (const QMimeData *mimeData = QGuiApplication::clipboard()->mimeData()) {
        if (mimeData->hasImage()) {
            const QImage image = qvariant_cast<QImage>(mimeData->imageData());
            if (!image.isNull())
                return image;
        }
    }
    return QImage();
}
#endif // !QT_NO_CLIPBOARD

void ImageViewer::paste()
{
#ifndef QT_NO_CLIPBOARD
    const QImage newImage = clipboardImage();
    if (newImage.isNull()) {
        statusBar()->showMessage(tr("No image in clipboard"));
    } else {
        setImage(newImage);
        setWindowFilePath(QString());
        const QString message = tr("Obtained image from clipboard, %1x%2, Depth: %3")
            .arg(newImage.width()).arg(newImage.height()).arg(newImage.depth());
        statusBar()->showMessage(message);
    }
#endif // !QT_NO_CLIPBOARD
}

void ImageViewer::zoomIn()
{
    scaleImage(1.25);
}

void ImageViewer::zoomOut()
{
    scaleImage(0.8);
}

void ImageViewer::normalSize()
{
    heightLabel->adjustSize();
    scaleFactor = 1.0;
}

void ImageViewer::fitToWindow()
{
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow)
        normalSize();
    updateActions();
}

void ImageViewer::about()
{
    QMessageBox::about(this, tr("About Image Viewer"),
            tr("<p>The <b>Image Viewer</b> example shows how to combine QLabel "
               "and QScrollArea to display an image. QLabel is typically used "
               "for displaying a text, but it can also display an image. "
               "QScrollArea provides a scrolling view around another widget. "
               "If the child widget exceeds the size of the frame, QScrollArea "
               "automatically provides scroll bars. </p><p>The example "
               "demonstrates how QLabel's ability to scale its contents "
               "(QLabel::scaledContents), and QScrollArea's ability to "
               "automatically resize its contents "
               "(QScrollArea::widgetResizable), can be used to implement "
               "zooming and scaling features. </p><p>In addition the example "
               "shows how to use QPainter to print an image.</p>"));
}

void ImageViewer::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *openAct = fileMenu->addAction(tr("&Open..."), this, &ImageViewer::open);
    openAct->setShortcut(QKeySequence::Open);

    saveAsAct = fileMenu->addAction(tr("&Save As..."), this, &ImageViewer::saveAs);
    saveAsAct->setEnabled(false);

    fileMenu->addSeparator();

    QAction *exitAct = fileMenu->addAction(tr("E&xit"), this, &QWidget::close);
    exitAct->setShortcut(tr("Ctrl+Q"));

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));

    copyAct = editMenu->addAction(tr("&Copy"), this, &ImageViewer::copy);
    copyAct->setShortcut(QKeySequence::Copy);
    copyAct->setEnabled(false);

    QAction *pasteAct = editMenu->addAction(tr("&Paste"), this, &ImageViewer::paste);
    pasteAct->setShortcut(QKeySequence::Paste);

    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));

    zoomInAct = viewMenu->addAction(tr("Zoom &In (25%)"), this, &ImageViewer::zoomIn);
    zoomInAct->setShortcut(QKeySequence::ZoomIn);
    zoomInAct->setEnabled(false);

    zoomOutAct = viewMenu->addAction(tr("Zoom &Out (25%)"), this, &ImageViewer::zoomOut);
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
    zoomOutAct->setEnabled(false);

    normalSizeAct = viewMenu->addAction(tr("&Normal Size"), this, &ImageViewer::normalSize);
    normalSizeAct->setShortcut(tr("Ctrl+S"));
    normalSizeAct->setEnabled(false);

    viewMenu->addSeparator();

    fitToWindowAct = viewMenu->addAction(tr("&Fit to Window"), this, &ImageViewer::fitToWindow);
    fitToWindowAct->setEnabled(false);
    fitToWindowAct->setCheckable(true);
    fitToWindowAct->setShortcut(tr("Ctrl+F"));

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    helpMenu->addAction(tr("&About"), this, &ImageViewer::about);
    helpMenu->addAction(tr("About &Qt"), &QApplication::aboutQt);
}

void ImageViewer::updateActions()
{
    saveAsAct->setEnabled(!image.isNull());
    copyAct->setEnabled(!image.isNull());
    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}

void ImageViewer::scaleImage(double factor)
{
    Q_ASSERT(heightLabel->pixmap());
    scaleFactor *= factor;
    heightLabel->resize(scaleFactor * heightLabel->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);
}

void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

