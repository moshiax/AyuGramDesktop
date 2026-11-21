// This is the source code of AyuGram for Desktop.
//
// We do not and cannot prevent the use of our code,
// but be respectful and credit the original author.
//
// Copyright @Radolyn, 2024
#include "ayu_logo.h"
#include "ayu/ayu_settings.h"
#include "core/launcher.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtGui/QImageReader>
#include <QtGui/QImageWriter>

static QString LAST_LOADED_NAME;
static QImage LAST_LOADED;
static QImage LAST_LOADED_NO_MARGIN;

namespace AyuAssets {

QString customAppIconPath() {
	return cWorkingDir() + u"tdata/ayu_custom_icon.ico"_q;
}

bool setCustomAppIcon(const QString &path) {
	const auto isPng = (QFileInfo(path).suffix().compare(
		u"png"_q,
		Qt::CaseInsensitive) == 0);
	const auto image = QImageReader(path).read();
	if (image.isNull()) {
		return false;
	}
	const auto destination = customAppIconPath();
	if (!isPng
		&& QFileInfo(path).absoluteFilePath() == QFileInfo(destination).absoluteFilePath()) {
		return true;
	}
	if (!QDir().mkpath(QFileInfo(destination).path())) {
		return false;
	}
	const auto temporary = destination + u".tmp"_q;
	QFile::remove(temporary);
	if (isPng) {
		QImageWriter writer(temporary, "ICO");
		if (!writer.write(image)) {
			QFile::remove(temporary);
			return false;
		}
	} else if (!QFile::copy(path, temporary)) {
		return false;
	}
	if (QFile::exists(destination) && !QFile::remove(destination)) {
		QFile::remove(temporary);
		return false;
	}
	if (!QFile::rename(temporary, destination)) {
		QFile::remove(temporary);
		return false;
	}
	LAST_LOADED_NAME.clear();
	return true;
}

void loadAppIco() {
	auto settings = &AyuSettings::getInstance();

	QString appDataPath = QDir::fromNativeSeparators(qgetenv("APPDATA"));
	QString tempIconPath = appDataPath + "/AyuGram.ico";

	// workaround for read-only file
	auto f = QFile(tempIconPath);
	if (f.exists()) {
		f.setPermissions(QFile::WriteOther);
		f.remove();
	}
	f.close();
	const auto source = (settings->appIcon == CUSTOM_ICON)
		? customAppIconPath()
		: qsl(":/gui/art/ayu/%1/app_icon.ico").arg(settings->appIcon);
	QFile::copy(source, tempIconPath);
}

void loadIcons() {
	auto settings = &AyuSettings::getInstance();
	if (LAST_LOADED_NAME != settings->appIcon) {
		LAST_LOADED_NAME = settings->appIcon;

		if (settings->appIcon == CUSTOM_ICON) {
			LAST_LOADED = QImageReader(customAppIconPath()).read();
			LAST_LOADED_NO_MARGIN = LAST_LOADED;
		} else {
			LAST_LOADED = QImage(qsl(":/gui/art/ayu/%1/app.png").arg(settings->appIcon));
			LAST_LOADED_NO_MARGIN = QImage(qsl(":/gui/art/ayu/%1/app_preview.png").arg(settings->appIcon));
		}
	}
}

QImage loadPreview(QString name) {
	return (name == CUSTOM_ICON)
		? QImageReader(customAppIconPath()).read()
		: QImage(qsl(":/gui/art/ayu/%1/app_preview.png").arg(name));
}

QString currentAppLogoName() {
	return LAST_LOADED_NAME;
}

QImage currentAppLogo() {
	loadIcons();
	return LAST_LOADED;
}

QImage currentAppLogoNoMargin() {
	loadIcons();
	return LAST_LOADED_NO_MARGIN;
}

}
