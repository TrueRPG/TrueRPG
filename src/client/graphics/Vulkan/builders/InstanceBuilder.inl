template <u32 version>
InstanceBuilder &InstanceBuilder::setAppVersion()
{
    m_info.appVersion = version;
    return *this;
}

template <u32 version>
InstanceBuilder &InstanceBuilder::setEngineVersion()
{
    m_info.engineVersion = version;
    return *this;
}

template <u32 version>
InstanceBuilder &InstanceBuilder::setApiVersion()
{
    m_info.apiVersion = version;
    return *this;
}