#pragma once

#include <iostream>

#include <cstdint>
#include <ctime>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include <utils/MeasureUnits/MeasureUnits.h>
#include <utils/emultiontypes.h>
#include <utils/modcod/modcod.h>

/**
 * @brief Структура заголовка пакета, содержащая метаданные
 *
 * Содержит идентификатор пакета, схему модуляции/кодирования,
 * временные параметры, частотные характеристики и размер полезной нагрузки.
 */
struct PacketHeader {
    /// Тип для уникального идентификатора пакета
    using Id = uint64_t;

    /// Необходимо заблокировать макрос max мигрировавший из какой-то из Си библиотек
    #undef max 
    /// Значение невалидного ID пакета
    static constexpr Id kInvalidId = std::numeric_limits<Id>::max();

    PacketHeader(Id id = kInvalidId,
                 ModCod::Type mod_cod = ModCod::Type::DUMMY_PLFRAME);

    /// Уникальный идентификатор пакета
    Id id;

    /// Тип модуляции и кодирования
    ModCod::Type mod_cod;

    /// Время, в которое надо отправить пакет после модулятора в нс
    MeasureUnits::Time timepos;

    /// Частота несущей в Гц
    MeasureUnits::Frequency carrier_frequency;

    /// Тактовая частота в Гц
    MeasureUnits::Frequency clock_frequency;

    /// Мощность
    MeasureUnits::Power power;

    /// Размер данных в байтах
    size_t size;

    /// Записывает заголовок в buf без выравнивания полей.
    /// \warning Минимальная длина buf должна быть sizeof(PacketHeader).
    /// \warning Все поля плотно упакованы. Используйте Unpack для распаковки.
    void Pack(uint8_t* buf) const {
        memcpy(buf, &id, sizeof(id));
        buf += sizeof(id);

        memcpy(buf, &mod_cod, sizeof(mod_cod));
        buf += sizeof(mod_cod);

        auto ns = timepos.asNanoseconds();
        memcpy(buf, &ns, sizeof(ns));
        buf += sizeof(ns);

        auto hertz = carrier_frequency.asHertz();
        memcpy(buf, &hertz, sizeof(hertz));
        buf += sizeof(hertz);

        memcpy(buf, &clock_frequency, sizeof(clock_frequency));
        buf += sizeof(clock_frequency);

        memcpy(buf, &power, sizeof(power));
        buf += sizeof(power);

        memcpy(buf, &size, sizeof(size));
        buf += sizeof(size);
    }

    /// Распаковывает заголовок.
    /// \warning buf должен быть получен из вызова Pack.
    void Unpack(const uint8_t* buf) {
        memcpy(&id, buf, sizeof(id));
        buf += sizeof(id);

        memcpy(&mod_cod, buf, sizeof(mod_cod));
        buf += sizeof(mod_cod);

        auto ns = timepos.asNanoseconds();
        memcpy(&ns, buf, sizeof(ns));
        timepos = MeasureUnits::Time(ns);
        buf += sizeof(ns);

        auto hertz = carrier_frequency.asHertz();
        memcpy(&hertz, buf, sizeof(hertz));
        carrier_frequency = MeasureUnits::Frequency(hertz);
        buf += sizeof(hertz);

        memcpy(&clock_frequency, buf, sizeof(clock_frequency));
        buf += sizeof(clock_frequency);

        memcpy(&power, buf, sizeof(power));
        buf += sizeof(power);

        memcpy(&size, buf, sizeof(size));
        buf += sizeof(size);
    }

    template <typename T = size_t>
    static constexpr T GetPackedSize() {
        size_t packed_size = 0;

        packed_size += sizeof(decltype(id));
        packed_size += sizeof(decltype(mod_cod));
        packed_size += sizeof(decltype(timepos.asNanoseconds()));
        packed_size += sizeof(decltype(carrier_frequency.asHertz()));
        packed_size += sizeof(decltype(clock_frequency));
        packed_size += sizeof(decltype(power));
        packed_size += sizeof(decltype(size));

        return static_cast<T>(packed_size);
    }
};

/*
 * @brief Структура пакета, содержащая заголовок и полезную нагрузку
 *
 * Представляет полный пакет данных с метаданными в заголовке и содержимым.
 */
struct Packet {
    /// Тип элементов полезной нагрузки
    using DataType = uint8_t;

    Packet(PacketHeader pk_header = PacketHeader());

    void SetPayload(const std::vector<DataType>& data) {
        this->payload = std::make_shared<std::vector<DataType>>(data);
        header.size = (*payload).size();
    }

    void SetPayload(std::vector<DataType>&& data) {
        this->payload =
            std::make_shared<std::vector<DataType>>(std::move(data));
        header.size = (*payload).size();
    }

    size_t Size() const { return header.size; }

    /// Длина нахождения пакета в сети
    MeasureUnits::Duration PacketDuration() const {
        return MeasureUnits::Duration((int64_t(header.size) * 1'000'000L) / header.clock_frequency.asHertz());
    }

    /// Заголовок пакета с метаданными
    PacketHeader header;

    /// Данные, передаваемы пакетом
    std::shared_ptr<std::vector<DataType>> payload;

    std::string ToString() {
        return "Packet\nid: " + std::to_string(static_cast<uint64_t>(header.id)) + 
               "\ntimepos: " + std::to_string(header.timepos.asNanoseconds()) +
               "\ncarrier_frequency: " + std::to_string(header.carrier_frequency.asHertz()) +
               "\nclock_frequency: " + std::to_string(header.clock_frequency.asHertz()) +
               "\npower: " + std::to_string(header.power.asMilliwatt()) + 
               "\nsize: " + std::to_string(header.size) +
               "\nduration: " + std::to_string(PacketDuration().asNanoseconds()) +
               '\n';
    }
};
