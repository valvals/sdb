#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <stddef.h>

namespace sdb{

// COMMON CONSTANTS
extern const char kApplicationEngName[];


// BASE VALUES ADDER DIALOG
//Common
extern const char kCreationTreeArrow[];
// Measurements atributes
extern const char kCreationTreeTableMeasurements[];
extern const char kCreationTreeInstrument[];
extern const char kCreationTreeSpectralDataTypes[];
extern const char kCreationTreeExperiment[];
extern const char kCreationTreeCaptureTypes[];
extern const char kCreationTreeOwners[];
// Classification atributes
extern const char kCreationTreeTableClassification[];
extern const char kCreationTreeGeneralType[];
extern const char kCreationTreeClass[];
extern const char kCreationTreeObject[];
// Location atributes
extern const char kCreationTreeTableLocation[];
extern const char kCreationTreeRegion[];
extern const char kCreationTreePlaceType[];
extern const char kCreationTreeGeographicName[];
extern const char kCreationTreeUniqueLocationName[];


// MESSAGES
// info
extern const char kMessageObjectSuccessfulyAddedToDB[];
extern const char kMessageObjectFailedAddingToDB[];
// warnings
extern const char kMessageWarningFieldLocationNameIsNotFilled[];
extern const char kMessageWarningFieldObjectNameIsNotFilled[];
extern const char kMessageWarningLocationNameIsNotUnique[];
extern const char kMessageWarningDontUseCommaAsDigitSeparator[];
extern const char kMessageWarningDontUseTABAsSeparator[];
extern const char kMessageWarningNoSpectralDataAdded[];
// errors
extern const char kMessageErrorDataBaseError[];
extern const char kMessageErrorAfterSeparatingElementsNumberLessThenTwo[];
extern const char kMessageErrorConvertionToDouble[];
extern const char kMessageErrorLineIsEmpty[];
extern const char kMessageErrorNoSpacesAsSeparatorFounded[];
}

#endif // CONSTANTS_H
