#pragma once

namespace Hilo
{
    // <snippet921>
    public interface class IResizable
    {
        property int ColumnSpan 
        {
            int get();
            void set(int value);
        }

        property int RowSpan
        {
            int get();
            void set(int value);
        }
    };
    // </snippet921>
}
