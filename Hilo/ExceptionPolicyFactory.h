#pragma once

namespace Hilo
{
    class ExceptionPolicy;

    class ExceptionPolicyFactory
    {
    public:
        static std::shared_ptr<ExceptionPolicy> GetCurrentPolicy();

        static std::shared_ptr<ExceptionPolicy> m_policy;
    };
}