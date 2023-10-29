#include "Constants.h"

namespace sdb{

// Common constants
const char kCreationTreeArrow[] = " -> ";
const char kApplicationEngName[] = "SPECTRABOX";

// Measurements atributes
const char kCreationTreeTableMeasurements[] = "атрибуты измерений";
const char kCreationTreeInstrument[] = "типы инструментов";
const char kCreationTreeSpectralDataTypes[] = "типы спектральных данных";
const char kCreationTreeExperiment[] = "названия экспериментов";
const char kCreationTreeCaptureTypes[] = "типы съёмки";
const char kCreationTreeOwners[] = "владельцы";

// Classification constants
const char kCreationTreeTableClassification[] = "классификация";
const char kCreationTreeGeneralType[] = "генеральный тип";
const char kCreationTreeClass[] = "класс";
const char kCreationTreeObject[] = "объект";

// Location constants
const char kCreationTreeTableLocation[] = "местоположение";
const char kCreationTreeRegion[] = "регион";
const char kCreationTreePlaceType[] = "тип места";
const char kCreationTreeGeographicName[] = "географическое название";
const char kCreationTreeUniqueLocationName[] = "уникальное место";

// MESSAGES
// info
const char kMessageObjectSuccessfulyAddedToDB[] = "объект был успешно добавлен в БД";
const char kMessageObjectFailedAddingToDB[] = "ошибка при добавлении объекта в БД";
// warnings
const char kMessageWarningFieldObjectNameIsNotFilled[] = "Обязательное поле 'объект' не заполнено";
const char kMessageWarningFieldLocationNameIsNotFilled[] = "Обязательное поле 'Название местоположения' не заполнено";
const char kMessageWarningDontUseCommaAsDigitSeparator[] = "Не используйте разделитель ',' для разрядов";
const char kMessageWarningDontUseTABAsSeparator[] = "Не используйте разделитель TAB";
const char kMessageWarningNoSpectralDataAdded[] = "Спектральные данные не добавлены.";
const char kMessageWarningLocationNameIsNotUnique[] = "Название местоположения не уникально!";
// errors
const char kMessageErrorDataBaseError[] = "Ошибка базы данных!";
const char kMessageErrorAfterSeparatingElementsNumberLessThenTwo[] = "После разделения количество элементов не равно 2";
const char kMessageErrorConvertionToDouble[] = "Ошибка преобразования в double";
const char kMessageErrorLineIsEmpty[] = "В файле есть пустая строка";
const char kMessageErrorNoSpacesAsSeparatorFounded[] = "Не найден разделитель (ПРОБЕЛ)";
}
