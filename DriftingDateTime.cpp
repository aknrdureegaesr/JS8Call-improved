#include <QMutexLocker>
#include <QLoggingCategory>

#include "DriftingDateTime.h"

Q_DECLARE_LOGGING_CATEGORY(driftingdatetime_js8)

DriftingDateTimeSingleton::DriftingDateTimeSingleton(): driftMS(0), mutex() {}

qint64 DriftingDateTimeSingleton::drift() {
    QMutexLocker locker(&mutex);
    return driftMS;
}

void DriftingDateTimeSingleton::setDriftInner(qint64 ms) {
    QMutexLocker locker(&mutex);
    driftMS = ms;
}

void DriftingDateTimeSingleton::setDrift(qint64 ms) {
    qint64 old_drift = drift();
    setDriftInner(ms);
    if(ms != old_drift) {
        qCDebug(driftingdatetime_js8) << "Changed drift from" << old_drift << "to" << ms << "ms";
        emit driftChanged(ms);
    } else {
        qCDebug(driftingdatetime_js8) << "Incoming signal without change of drift, still" << old_drift << "ms";
    }
};

DriftingDateTimeSingleton DriftingDateTimeSingleton::singleton = {};

Q_LOGGING_CATEGORY(driftingdatetime_js8, "driftingdatetime.js8", QtWarningMsg)
