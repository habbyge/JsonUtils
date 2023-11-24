

#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include "rapidjson/document.h"
#include "rapidjson/encodings.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/prettywriter.h"  // for stringify JSON
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

using namespace rapidjson;
//ʹ��UTF-16��doc,��rapidjson�����ռ������UTF8��Document��Value�Ķ���,���ǲ�����UTF16��
typedef rapidjson::GenericDocument<rapidjson::UTF16<>> DocumentW;
typedef rapidjson::GenericValue<rapidjson::UTF16<>> ValueW;
typedef rapidjson::GenericStringStream<rapidjson::UTF16<>> StringStreamW;
typedef rapidjson::GenericStringBuffer<rapidjson::UTF16<>> StringBufferW;

/**
 * Json�����࣬��װJson�����Ĳ���ȷ���
 * ʹ�÷������£�
 * CJsonUtils json;
 * json["a"] = "a";
 * json["b"] = "123";
 * json["c"] = 123;
 * json["d"] = "1.5";
 * json["e"] = 1.5
 * json["child"]["a"] = "1";
 * json["child"]["b"] = "2";
 * json["array"][0] = 1;
 * json["array"][1] = 2;
 *
 * std::string strJsonText = json.GetJsonText();
 *
 * CJsonUtils json2;
 * json2.Parse(strJsonText);
 *
 * std::string a1 = json2["a"];
 * int a2 = json2["a"];
 * double a3 = json2["a"];
 * int ca1 = json["child"]["a"];
 * int aa1 = json["array"][0];
 */
class CJsonUtils {
 private:
  CJsonUtils(rapidjson::Document* pDoc, rapidjson::Value* pValue);

 public:
  CJsonUtils();
  CJsonUtils(const CJsonUtils& other);
  ~CJsonUtils();

  CJsonUtils& operator=(const CJsonUtils& Other);

  /**
   * ����Json�ı�
   * @param strJsonText
   * @return
   */
  bool Parse(const char* strJsonText);
  bool Parse(const std::string& strJsonText);

  /**
   * ����Json����
   * @return Json�ı��ַ���
   */
  std::string GetJsonText(bool isPretty = false) const;

 public:
  /**
   * �жϵ�ǰ�洢�����ݵ�ʲô����
   * IsNumber = IsInt32|IsInt64|IsUint32|IsUint64|IsDouble
   *
   * @return
   *		true	��
   *		false	����
   *
   * ע��:
   *		1.�ַ������֣�����Ϊ�ַ��������������֣��������ȷ�Ĵ洢ֵ������
   *		2.�������ζ��ԣ���ֵ����ȷת��Ϊĳ�����ͣ���IsType=true.����Ϊ����:
   *			 1:		IsInt32��IsInt64��IsUint32��IsUint64 =
   *true -1:		IsInt32��IsInt64   = true; IsUint32��IsUint64 = false
   *			50��:	IsInt64��IsUint64  = true;
   *					IsInt32�� IsUint32 = false
   */
  bool IsObject() const;
  bool IsString() const;
  bool IsNumber() const;
  bool IsStringNumber() const;
  bool IsInt32() const;
  bool IsInt64() const;
  bool IsUint32() const;
  bool IsUint64() const;
  bool IsDouble() const;
  bool IsBool() const;

  /**
   * �ж��Ƿ�����ƶ��ļ�ֵ��
   * @param[in] strKey ���ַ���
   * @return
   */
  bool IsExists(const char* strKey) const;
  bool IsExists(const std::string& strKey) const;

  /**
   * ɾ��ָ����ֵ��
   * @param[in] strKey ���ַ���
   * @return
   */
  bool Remove(const char* strKey) const;
  bool Remove(const std::string& strKey) const;

  /**
   * ɾ��ָ������Ԫ��
   * @param[in] iArrayIndex ����������
   * @return
   */
  bool Remove(int32_t iArrayIndex) const;

 public:
  /**
   * ��ֵ�Բ������õ�ֵ����
   * @param[in] strKey ���ַ���
   * @return ֵ����
   */
  CJsonUtils operator[](const char* strKey);
  CJsonUtils operator[](const std::string& strKey);
  const CJsonUtils operator[](const char* strKey) const;
  const CJsonUtils operator[](const std::string& strKey) const;

  /**
   * ����������Ԫ�صĸ���
   * @return
   *		>=0	Ԫ�ظ���
   *		<0	��������
   */
  int32_t ArrayCount() const;

  /**
   * �õ�������ָ�������Ķ���
   * @param[in] iArrayIndex ����������
   * @return ֵ����
   */
  CJsonUtils operator[](int32_t iArrayIndex);
  const CJsonUtils operator[](int32_t iArrayIndex) const;

 public:
  /**
   * ��ֵ�������޸Ķ���洢������
   * @param[in] ����Value
   * @return ��������������
   */
  CJsonUtils& operator=(const char* strValue);
  CJsonUtils& operator=(const std::string& strValue);
  CJsonUtils& operator=(int32_t iValue);
  CJsonUtils& operator=(int64_t i64Value);
  CJsonUtils& operator=(uint32_t uValue);
  CJsonUtils& operator=(uint64_t u64Value);
  CJsonUtils& operator=(double dValue);
  CJsonUtils& operator=(bool bValue);

  /**
   * ��ʽת����ȡֵ
   * @return
   *		std::string	���洢�����ַ�������ĵõ��ַ���
   *					���洢������ֵ����õ�ת������ַ���
   *					���洢��������������õ�Json�ı�
   *
   *		int32_t��uint32_t��int64_t��uint64_t��double
   *					���洢������ֵ���򷵻���ֵ����
   *					���洢�����ַ�����ʽ����ֵ�����Զ�ת��Ϊ��Ӧ����ֵ
   *					���洢�����������ͣ��򷵻�0
   *
   * ע��:
   *		ȡֵ�����ƻ�ԭ���Ĵ洢���ͣ�����:ԭ���洢�� "1234" -
   *�ַ���1234������int��ȡֵ�� �õ�����Ȼ�����ε�1234�����Ǵ洢�����ݻ����ַ���1234������ı�
   */
  operator std::string() const;
  operator int32_t() const;
  operator uint32_t() const;
  operator int64_t() const;
  operator uint64_t() const;
  operator double() const;
  operator bool() const;

 private:
  template <typename _TYPE_, typename _CONVERT_FUNC_, typename _DEF_VAL>
  _TYPE_ ConvertToNumber(_CONVERT_FUNC_ pFunc, _DEF_VAL tDefValue) const;

  bool m_bRoot;
  rapidjson::Document* m_pDoc;
  rapidjson::Value* m_pValue;
};

class CJsonUtilsW {
 private:
  CJsonUtilsW(DocumentW* pDoc, ValueW* pValue);

 public:
  CJsonUtilsW();
  CJsonUtilsW(const CJsonUtilsW& other);
  ~CJsonUtilsW();

  CJsonUtilsW& operator=(const CJsonUtilsW& Other);

  /**
   * ����Json�ı�
   * @param strJsonText
   * @return
   */
  bool Parse(const wchar_t* strJsonText);
  bool Parse(const std::wstring& strJsonText);

  /**
   * ����Json����
   * @return Json�ı��ַ���
   */
  std::wstring GetJsonText(bool isPretty = false) const;

 public:
  /**
   * �жϵ�ǰ�洢�����ݵ�ʲô����
   * IsNumber = IsInt32|IsInt64|IsUint32|IsUint64|IsDouble
   *
   * @return
   *		true	��
   *		false	����
   *
   * ע��:
   *		1.�ַ������֣�����Ϊ�ַ��������������֣��������ȷ�Ĵ洢ֵ������
   *		2.�������ζ��ԣ���ֵ����ȷת��Ϊĳ�����ͣ���IsType=true.����Ϊ����:
   *			 1:		IsInt32��IsInt64��IsUint32��IsUint64 =
   *true -1:		IsInt32��IsInt64   = true; IsUint32��IsUint64 = false
   *			50��:	IsInt64��IsUint64  = true;
   *					IsInt32�� IsUint32 = false
   */
  bool IsObject() const;
  bool IsString() const;
  bool IsNumber() const;
  bool IsStringNumber() const;
  bool IsInt32() const;
  bool IsInt64() const;
  bool IsUint32() const;
  bool IsUint64() const;
  bool IsDouble() const;
  bool IsBool() const;

  /**
   * �ж��Ƿ�����ƶ��ļ�ֵ��
   * @param[in] strKey ���ַ���
   * @return
   */
  bool IsExists(const wchar_t* strKey) const;
  bool IsExists(const std::wstring& strKey) const;

  /**
   * ɾ��ָ����ֵ��
   * @param[in] strKey ���ַ���
   * @return
   */
  bool Remove(const wchar_t* strKey) const;
  bool Remove(const std::wstring& strKey) const;

  /**
   * ɾ��ָ������Ԫ��
   * @param[in] iArrayIndex ����������
   * @return
   */
  bool Remove(int32_t iArrayIndex) const;

 public:
  /**
   * ��ֵ�Բ������õ�ֵ����
   * @param[in] strKey ���ַ���
   * @return ֵ����
   */
  CJsonUtilsW operator[](const wchar_t* strKey);
  CJsonUtilsW operator[](const std::wstring& strKey);
  const CJsonUtilsW operator[](const wchar_t* strKey) const;
  const CJsonUtilsW operator[](const std::wstring& strKey) const;

  /**
   * ����������Ԫ�صĸ���
   * @return
   *		>=0	Ԫ�ظ���
   *		<0	��������
   */
  int32_t ArrayCount() const;

  /**
   * �õ�������ָ�������Ķ���
   * @param[in] iArrayIndex ����������
   * @return ֵ����
   */
  CJsonUtilsW operator[](int32_t iArrayIndex);
  const CJsonUtilsW operator[](int32_t iArrayIndex) const;

 public:
  /**
   * ��ֵ�������޸Ķ���洢������
   * @param[in] ����Value
   * @return ��������������
   */
  CJsonUtilsW& operator=(const wchar_t* strValue);
  CJsonUtilsW& operator=(const std::wstring& strValue);
  CJsonUtilsW& operator=(int32_t iValue);
  CJsonUtilsW& operator=(int64_t i64Value);
  CJsonUtilsW& operator=(uint32_t uValue);
  CJsonUtilsW& operator=(uint64_t u64Value);
  CJsonUtilsW& operator=(double dValue);
  CJsonUtilsW& operator=(bool bValue);

  /**
   * ��ʽת����ȡֵ
   * @return
   *		std::string	���洢�����ַ�������ĵõ��ַ���
   *					���洢������ֵ����õ�ת������ַ���
   *					���洢��������������õ�Json�ı�
   *
   *		int32_t��uint32_t��int64_t��uint64_t��double
   *					���洢������ֵ���򷵻���ֵ����
   *					���洢�����ַ�����ʽ����ֵ�����Զ�ת��Ϊ��Ӧ����ֵ
   *					���洢�����������ͣ��򷵻�0
   *
   * ע��:
   *		ȡֵ�����ƻ�ԭ���Ĵ洢���ͣ�����:ԭ���洢�� "1234" -
   *�ַ���1234������int��ȡֵ�� �õ�����Ȼ�����ε�1234�����Ǵ洢�����ݻ����ַ���1234������ı�
   */
  operator std::wstring() const;
  operator int32_t() const;
  operator uint32_t() const;
  operator int64_t() const;
  operator uint64_t() const;
  operator double() const;
  operator bool() const;

 private:
  template <typename _TYPE_, typename _CONVERT_FUNC_, typename _DEF_VAL>
  _TYPE_ ConvertToNumber(_CONVERT_FUNC_ pFunc, _DEF_VAL tDefValue) const;

  bool m_bRoot;
  DocumentW* m_pDoc;
  ValueW* m_pValue;
};

#endif  // JSON_UTILS_H