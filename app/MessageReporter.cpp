#include "MessageReporter.h"
#include "QMessageBox"
#include "Constants.h"
#include <stddef.h>

namespace  {

void showMessage(const QString& message, QMessageBox::Icon icon){
    QMessageBox mb;
    mb.setWindowTitle(sdb::kApplicationEngName);
    mb.setInformativeText(message);
    mb.setIcon(icon);
    mb.exec();
};
void showWarnigMessage(const QString& message){
    showMessage(message,QMessageBox::Warning);
};
void showErrorMessage(const QString& message){
    showMessage(message,QMessageBox::Critical);
}
void showInfoMessage(const QString& message){
    showMessage(message,QMessageBox::Information);
}
}

namespace sdb{

void showError(ERRORS error_msg)
{
    using namespace sdb;
    switch (error_msg) {
    case sdb::DATA_BASE_ERROR:
        showErrorMessage(kMessageErrorDataBaseError);
        break;
    case LINE_IS_EMPTY:
        showErrorMessage(kMessageErrorLineIsEmpty);
        break;
    case NO_SPACE_IN_LINE_ERROR:
        showErrorMessage(kMessageErrorNoSpacesAsSeparatorFounded);
        break;
    case PARSE_TO_VALUE_ERROR:
        showErrorMessage(kMessageErrorConvertionToDouble);
        break;
    case NO_2_ELEMNTS_ERROR:
        showErrorMessage(kMessageErrorAfterSeparatingElementsNumberLessThenTwo);
        break;
    case FAIL_TO_ADD_OBJECT_TO_DB:
        showErrorMessage(kMessageObjectFailedAddingToDB);
        break;
    }
}

void showWarning(WARNINGS warning_msg)
{
    switch(warning_msg){
    case sdb::LOCATION_NAME_IS_NOT_UNIQUE:
        showWarnigMessage(kMessageWarningLocationNameIsNotUnique);
        break;
    case sdb::NO_SPECTRAL_DATA_ADDED:
        showWarnigMessage(kMessageWarningNoSpectralDataAdded);
        break;
    case NO_LOCATION_NAME_FILLED:
        showWarnigMessage(kMessageWarningFieldLocationNameIsNotFilled);
        break;
    case NO_OBJECT_FILLED:
        showWarnigMessage(kMessageWarningFieldObjectNameIsNotFilled);
        break;
    case TAB_SEPARATOR:
        showWarnigMessage(kMessageWarningDontUseTABAsSeparator);
        break;
    case COMMA_DECIMAL:
        showWarnigMessage(kMessageWarningDontUseCommaAsDigitSeparator);
        break;
    }
}

void showInfo(INFO info_msg)
{
    switch (info_msg) {
    case NEW_OBJECT_SUCCESSFULLY_ADDED_TO_DB:
        showInfoMessage(kMessageObjectSuccessfulyAddedToDB);
        break;
    }
}

}
