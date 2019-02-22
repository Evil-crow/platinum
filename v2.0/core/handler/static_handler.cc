/**
 * Created by Crow on 2/9/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  
 */

#include "core/handler/static_handler.h"

#include <ctime>
#include <cstring>
#include "net/connection.h"
#include "protocol/http/request.h"
#include "config/config.h"
#include "utility/file_reader.h"
#include "utility/dir.h"
#include "utility/logger.h"

using namespace platinum;

StaticHandler::StaticHandler(Connection *connection,
                             const http::Request &request,
                             const std::map<std::string, std::string> &parameters,
                             const std::string &file,
                             const std::string &path) noexcept
    : Handler(connection, request, parameters, file, path),
      file_size_(-1)
{
  ;
}

void StaticHandler::Serve()
{
  // 1.Set file from the URL
  const auto &config = Config::GetInstance();
  auto www_root = config.www_root();
  auto default_root = config.default_root();

  if (file_.empty())
    file_ = config.index();

  if (IsValid()) {
    if (IsExist()) {
      if (!Forbidden()) {
        builder_->SetStatusCode(200);
        auto new_file = www_root + path_ + "/" + file_;
        SetNewFile(new_file);
      } else {
        builder_->SetStatusCode(403);
        auto new_file = default_root + "/403.html";
        SetNewFile(new_file);
      }
    } else {
      builder_->SetStatusCode(404);
      auto new_file = default_root + "/404.html";
      SetNewFile(new_file);
    }
  } else {
    builder_->SetStatusCode(501);
    auto new_file = default_root + "/501.html";
    SetNewFile(new_file);
  }

  // 2. Set Basic Headers
  SetDateHeader();
  SetServerHeader();
  SetConnectionHeader();
  SetContentTypeHeader();
  builder_->SetVersionMajor(1);
  builder_->SetVersionMinor(1);

  // 3. Serve the static resource
  {
    FileReader reader(file_);
    file_size_ = reader.Size();
  }
  builder_->SetHeader("Content-Length", std::to_string(file_size_));

  if (file_size_ < 500000) {
    ServerByWrite();
  } else {
    ServerBySendFile();
  }

  // 4. Deal with persistent connection, Close => Force close, Keep-Alive => wait
  if (request_.header("Connection") == "kee-alive") {
    ;                                           // keep-alive => nothing to do
  } else {
    connection_->ForceClose();
  }
  LOG(INFO) << "Serve " + request_.url() + " => " + file_;
}

bool StaticHandler::ServerByWrite()
{
  // Serve the response header
  auto resposne = builder_->GetResponse();
  auto data = resposne.Build();
  connection_->SendData(data.data(), data.size());

  if (request_.method() == "HEAD") {
    return true;
  } else {
    // Serve the response body by write(2)
    ssize_t num;
    unsigned char buffer[1024];

    FileReader reader(file_);
    while (!reader.Complete()) {
      num = reader.Read(buffer, 1024);
      if (num >= 0)
        connection_->SendData(buffer, static_cast<size_t>(num));
    }
  }
}

bool StaticHandler::ServerBySendFile()
{
  // Serve the response header
  auto resposne = builder_->GetResponse();
  auto data = resposne.Build();
  connection_->SendData(data.data(), data.size());

  if (request_.method() == "HEAD") {
    return true;
  } else {
    // Serve the response body by sendfile(2)
    connection_->SendFile(file_, static_cast<size_t>(file_size_));
  }
}

// Header: Date
void StaticHandler::SetDateHeader()
{
  char time[64];                      // 64 Bytes is already enough !
  auto t = std::time(nullptr);
  std::strftime(time, sizeof(time), "%a, %d %b %Y %H:%M:%S GMT",std::localtime(&t));

  builder_->SetHeader("Date", time);
}

// Header: Connection
void StaticHandler::SetConnectionHeader()
{
  builder_->SetHeader("Connection", request_.header("Connection"));
}

// Header: Content-Type
void StaticHandler::SetContentTypeHeader()
{
  auto suffix = std::string(file_, file_.find('.'));
  builder_->SetHeader("Content-Type", MIMEType(suffix.c_str()));
}

void StaticHandler::SetServerHeader()
{
  builder_->SetHeader("Server", "platinum/2.0");
}

const char *StaticHandler::MIMEType(const char *str)
{
#define TYPE(suffix, mime) if (strcmp(str, suffix) == 0) return mime;
  TYPE (".tif", "image/tiff");
  TYPE (".001", "application/x-001");
  TYPE (".301", "application/x-301");
  TYPE (".323", "text/h323");
  TYPE (".906", "application/x-906");
  TYPE (".907", "drawing/907");
  TYPE (".a11", "application/x-a11");
  TYPE (".acp", "audio/x-mei-aac");
  TYPE (".ai", "application/postscript");
  TYPE (".aif", "audio/aiff");
  TYPE (".aifc", "audio/aiff");
  TYPE (".aiff", "audio/aiff");
  TYPE (".anv", "application/x-anv");
  TYPE (".asa", "text/asa");
  TYPE (".asf", "video/x-ms-asf");
  TYPE (".asp", "text/asp");
  TYPE (".asx", "video/x-ms-asf");
  TYPE (".au", "audio/basic");
  TYPE (".avi", "video/avi");
  TYPE (".awf", "application/vnd.adobe.workflow");
  TYPE (".biz", "text/xml");
  TYPE (".bmp", "application/x-bmp");
  TYPE (".bot", "application/x-bot");
  TYPE (".c4t", "application/x-c4t");
  TYPE (".c90", "application/x-c90");
  TYPE (".cal", "application/x-cals");
  TYPE (".cat", "application/vnd.ms-pki.seccat");
  TYPE (".cdf", "application/x-netcdf");
  TYPE (".cdr", "application/x-cdr");
  TYPE (".cel", "application/x-cel");
  TYPE (".cer", "application/x-x509-ca-cert");
  TYPE (".cg4", "application/x-g4");
  TYPE (".cgm", "application/x-cgm");
  TYPE (".cit", "application/x-cit");
  TYPE (".class", "java/*");
  TYPE (".cml", "text/xml");
  TYPE (".cmp", "application/x-cmp");
  TYPE (".cmx", "application/x-cmx");
  TYPE (".cot", "application/x-cot");
  TYPE (".crl", "application/pkix-crl");
  TYPE (".crt", "application/x-x509-ca-cert");
  TYPE (".csi", "application/x-csi");
  TYPE (".css", "text/css");
  TYPE (".cut", "application/x-cut");
  TYPE (".dbf", "application/x-dbf");
  TYPE (".dbm", "application/x-dbm");
  TYPE (".dbx", "application/x-dbx");
  TYPE (".dcd", "text/xml");
  TYPE (".dcx", "application/x-dcx");
  TYPE (".der", "application/x-x509-ca-cert");
  TYPE (".dgn", "application/x-dgn");
  TYPE (".dib", "application/x-dib");
  TYPE (".dll", "application/x-msdownload");
  TYPE (".doc", "application/msword");
  TYPE (".dot", "application/msword");
  TYPE (".drw", "application/x-drw");
  TYPE (".dtd", "text/xml");
  TYPE (".dwf", "Model/vnd.dwf");
  TYPE (".dwf", "application/x-dwf");
  TYPE (".dwg", "application/x-dwg");
  TYPE (".dxb", "application/x-dxb");
  TYPE (".dxf", "application/x-dxf");
  TYPE (".edn", "application/vnd.adobe.edn");
  TYPE (".emf", "application/x-emf");
  TYPE (".eml", "message/rfc822");
  TYPE (".ent", "text/xml");
  TYPE (".epi", "application/x-epi");
  TYPE (".eps", "application/x-ps");
  TYPE (".eps", "application/postscript");
  TYPE (".etd", "application/x-ebx");
  TYPE (".exe", "application/x-msdownload");
  TYPE (".fax", "image/fax");
  TYPE (".fdf", "application/vnd.fdf");
  TYPE (".fif", "application/fractals");
  TYPE (".fo", "text/xml");
  TYPE (".frm", "application/x-frm");
  TYPE (".g4", "application/x-g4");
  TYPE (".gbr", "application/x-gbr");
  TYPE (".gif", "image/gif");
  TYPE (".gl2", "application/x-gl2");
  TYPE (".gp4", "application/x-gp4");
  TYPE (".hgl", "application/x-hgl");
  TYPE (".hmr", "application/x-hmr");
  TYPE (".hpg", "application/x-hpgl");
  TYPE (".hpl", "application/x-hpl");
  TYPE (".hqx", "application/mac-binhex40");
  TYPE (".hrf", "application/x-hrf");
  TYPE (".hta", "application/hta");
  TYPE (".htc", "text/x-component");
  TYPE (".htm", "text/html");
  TYPE (".html", "text/html");
  TYPE (".htt", "text/webvTYPEwhtml");
  TYPE (".htx", "text/html");
  TYPE (".icb", "application/x-icb");
  TYPE (".ico", "image/x-icon");
  TYPE (".ico", "application/x-ico");
  TYPE (".iff", "application/x-iff");
  TYPE (".ig4", "application/x-g4");
  TYPE (".igs", "application/x-igs");
  TYPE (".iii", "application/x-iphone");
  TYPE (".img", "application/x-img");
  TYPE (".ins", "application/x-internet-signup");
  TYPE (".isp", "application/x-internet-signup");
  TYPE (".IVF", "video/x-ivf");
  TYPE (".java", "java/*");
  TYPE (".jfif", "image/jpeg");
  TYPE (".jpe", "image/jpeg");
  TYPE (".jpe", "application/x-jpe");
  TYPE (".jpeg", "image/jpeg");
  TYPE (".jpg", "image/jpeg");
  TYPE (".jpg", "application/x-jpg");
  TYPE (".js", "application/javascript");
  TYPE (".la1", "audio/x-liquid-file");
  TYPE (".lar", "application/x-laplayer-reg");
  TYPE (".latex", "application/x-latex");
  TYPE (".lavs", "audio/x-liquid-secure");
  TYPE (".lbm", "application/x-lbm");
  TYPE (".lmsff", "audio/x-la-lms");
  TYPE (".ls", "application/x-javascript");
  TYPE (".ltr", "application/x-ltr");
  TYPE (".m1v", "video/x-mpeg");
  TYPE (".m2v", "video/x-mpeg");
  TYPE (".m3u", "audio/mpegurl");
  TYPE (".m4e", "video/mpeg4");
  TYPE (".mac", "application/x-mac");
  TYPE (".man", "application/x-troff-man");
  TYPE (".math", "text/xml");
  TYPE (".mdb", "application/msaccess");
  TYPE (".mdb", "application/x-mdb");
  TYPE (".mfp", "application/x-shockwave-flash");
  TYPE (".mht", "message/rfc822");
  TYPE (".mhtml", "message/rfc822");
  TYPE (".mi", "application/x-mi");
  TYPE (".mid", "audio/mid");
  TYPE (".midi", "audio/mid");
  TYPE (".mil", "application/x-mil");
  TYPE (".mml", "text/xml");
  TYPE (".mnd", "audio/x-musicnet-download");
  TYPE (".mns", "audio/x-musicnet-stream");
  TYPE (".mocha", "application/x-javascript");
  TYPE (".movTYPE", "video/x-sgi-movTYPE");
  TYPE (".mp1", "audio/mp1");
  TYPE (".mp2", "audio/mp2");
  TYPE (".mp2v", "video/mpeg");
  TYPE (".mp3", "audio/mp3");
  TYPE (".mp4", "video/mpeg4");
  TYPE (".mpa", "video/x-mpg");
  TYPE (".mpd", "application/vnd.ms-project");
  TYPE (".mpe", "video/x-mpeg");
  TYPE (".mpeg", "video/mpg");
  TYPE (".mpg", "video/mpg");
  TYPE (".mpga", "audio/rn-mpeg");
  TYPE (".mpp", "application/vnd.ms-project");
  TYPE (".mps", "video/x-mpeg");
  TYPE (".mpt", "application/vnd.ms-project");
  TYPE (".mpv", "video/mpg");
  TYPE (".mpv2", "video/mpeg");
  TYPE (".mpw", "application/vnd.ms-project");
  TYPE (".mpx", "application/vnd.ms-project");
  TYPE (".mtx", "text/xml");
  TYPE (".mxp", "application/x-mmxp");
  TYPE (".net", "image/pnetvue");
  TYPE (".nrf", "application/x-nrf");
  TYPE (".nws", "message/rfc822");
  TYPE (".odc", "text/x-ms-odc");
  TYPE (".out", "application/x-out");
  TYPE (".p10", "application/pkcs10");
  TYPE (".p12", "application/x-pkcs12");
  TYPE (".p7b", "application/x-pkcs7-certificates");
  TYPE (".p7c", "application/pkcs7-mime");
  TYPE (".p7m", "application/pkcs7-mime");
  TYPE (".p7r", "application/x-pkcs7-certreqresp");
  TYPE (".p7s", "application/pkcs7-signature");
  TYPE (".pc5", "application/x-pc5");
  TYPE (".pci", "application/x-pci");
  TYPE (".pcl", "application/x-pcl");
  TYPE (".pcx", "application/x-pcx");
  TYPE (".pdf", "application/pdf");
  TYPE (".pdf", "application/pdf");
  TYPE (".pdx", "application/vnd.adobe.pdx");
  TYPE (".pfx", "application/x-pkcs12");
  TYPE (".pgl", "application/x-pgl");
  TYPE (".pic", "application/x-pic");
  TYPE (".pko", "application/vnd.ms-pki.pko");
  TYPE (".pl", "application/x-perl");
  TYPE (".plg", "text/html");
  TYPE (".pls", "audio/scpls");
  TYPE (".plt", "application/x-plt");
  TYPE (".png", "image/png");
  TYPE (".png", "application/x-png");
  TYPE (".pot", "application/vnd.ms-powerpoint");
  TYPE (".ppa", "application/vnd.ms-powerpoint");
  TYPE (".ppm", "application/x-ppm");
  TYPE (".pps", "application/vnd.ms-powerpoint");
  TYPE (".ppt", "application/vnd.ms-powerpoint");
  TYPE (".ppt", "application/x-ppt");
  TYPE (".pr", "application/x-pr");
  TYPE (".prf", "application/pics-rules");
  TYPE (".prn", "application/x-prn");
  TYPE (".prt", "application/x-prt");
  TYPE (".ps", "application/x-ps");
  TYPE (".ps", "application/postscript");
  TYPE (".ptn", "application/x-ptn");
  TYPE (".pwz", "application/vnd.ms-powerpoint");
  TYPE (".r3t", "text/vnd.rn-realtext3d");
  TYPE (".ra", "audio/vnd.rn-realaudio");
  TYPE (".ram", "audio/x-pn-realaudio");
  TYPE (".ras", "application/x-ras");
  TYPE (".rat", "application/rat-file");
  TYPE (".rdf", "text/xml");
  TYPE (".rec", "application/vnd.rn-recording");
  TYPE (".red", "application/x-red");
  TYPE (".rgb", "application/x-rgb");
  TYPE (".rjs", "application/vnd.rn-realsystem-rjs");
  TYPE (".rjt", "application/vnd.rn-realsystem-rjt");
  TYPE (".rlc", "application/x-rlc");
  TYPE (".rle", "application/x-rle");
  TYPE (".rm", "application/vnd.rn-realmedia");
  TYPE (".rmf", "application/vnd.adobe.rmf");
  TYPE (".rmi", "audio/mid");
  TYPE (".rmj", "application/vnd.rn-realsystem-rmj");
  TYPE (".rmm", "audio/x-pn-realaudio");
  TYPE (".rmp", "application/vnd.rn-rn_music_package");
  TYPE (".rms", "application/vnd.rn-realmedia-secure");
  TYPE (".rmvb", "application/vnd.rn-realmedia-vbr");
  TYPE (".rmx", "application/vnd.rn-realsystem-rmx");
  TYPE (".rnx", "application/vnd.rn-realplayer");
  TYPE (".rp", "image/vnd.rn-realpix");
  TYPE (".rpm", "audio/x-pn-realaudio-plugin");
  TYPE (".rsml", "application/vnd.rn-rsml");
  TYPE (".rt", "text/vnd.rn-realtext");
  TYPE (".rtf", "application/msword");
  TYPE (".rtf", "application/x-rtf");
  TYPE (".rv", "video/vnd.rn-realvideo");
  TYPE (".sam", "application/x-sam");
  TYPE (".sat", "application/x-sat");
  TYPE (".sdp", "application/sdp");
  TYPE (".sdw", "application/x-sdw");
  TYPE (".sit", "application/x-stuffit");
  TYPE (".slb", "application/x-slb");
  TYPE (".sld", "application/x-sld");
  TYPE (".slk", "drawing/x-slk");
  TYPE (".smi", "application/smil");
  TYPE (".smil", "application/smil");
  TYPE (".smk", "application/x-smk");
  TYPE (".snd", "audio/basic");
  TYPE (".sol", "text/plain");
  TYPE (".sor", "text/plain");
  TYPE (".spc", "application/x-pkcs7-certificates");
  TYPE (".spl", "application/futuresplash");
  TYPE (".spp", "text/xml");
  TYPE (".ssm", "application/streamingmedia");
  TYPE (".sst", "application/vnd.ms-pki.certstore");
  TYPE (".stl", "application/vnd.ms-pki.stl");
  TYPE (".stm", "text/html");
  TYPE (".sty", "application/x-sty");
  TYPE (".svg", "text/xml");
  TYPE (".swf", "application/x-shockwave-flash");
  TYPE (".tdf", "application/x-tdf");
  TYPE (".tg4", "application/x-tg4");
  TYPE (".tga", "application/x-tga");
  TYPE (".tif", "image/tiff");
  TYPE (".tif", "application/x-tif");
  TYPE (".tiff", "image/tiff");
  TYPE (".tld", "text/xml");
  TYPE (".top", "drawing/x-top");
  TYPE (".torrent", "application/x-bittorrent");
  TYPE (".tsd", "text/xml");
  TYPE (".txt", "text/plain");
  TYPE (".uin", "application/x-icq");
  TYPE (".uls", "text/iuls");
  TYPE (".vcf", "text/x-vcard");
  TYPE (".vda", "application/x-vda");
  TYPE (".vdx", "application/vnd.visio");
  TYPE (".vml", "text/xml");
  TYPE (".vpg", "application/x-vpeg005");
  TYPE (".vsd", "application/vnd.visio");
  TYPE (".vsd", "application/x-vsd");
  TYPE (".vss", "application/vnd.visio");
  TYPE (".vst", "application/vnd.visio");
  TYPE (".vst", "application/x-vst");
  TYPE (".vsw", "application/vnd.visio");
  TYPE (".vsx", "application/vnd.visio");
  TYPE (".vtx", "application/vnd.visio");
  TYPE (".vxml", "text/xml");
  TYPE (".wav", "audio/wav");
  TYPE (".wax", "audio/x-ms-wax");
  TYPE (".wb1", "application/x-wb1");
  TYPE (".wb2", "application/x-wb2");
  TYPE (".wb3", "application/x-wb3");
  TYPE (".wbmp", "image/vnd.wap.wbmp");
  TYPE (".wiz", "application/msword");
  TYPE (".wk3", "application/x-wk3");
  TYPE (".wk4", "application/x-wk4");
  TYPE (".wkq", "application/x-wkq");
  TYPE (".wks", "application/x-wks");
  TYPE (".wm", "video/x-ms-wm");
  TYPE (".wma", "audio/x-ms-wma");
  TYPE (".wmd", "application/x-ms-wmd");
  TYPE (".wmf", "application/x-wmf");
  TYPE (".wml", "text/vnd.wap.wml");
  TYPE (".wmv", "video/x-ms-wmv");
  TYPE (".wmx", "video/x-ms-wmx");
  TYPE (".wmz", "application/x-ms-wmz");
  TYPE (".wp6", "application/x-wp6");
  TYPE (".wpd", "application/x-wpd");
  TYPE (".wpg", "application/x-wpg");
  TYPE (".wpl", "application/vnd.ms-wpl");
  TYPE (".wq1", "application/x-wq1");
  TYPE (".wr1", "application/x-wr1");
  TYPE (".wri", "application/x-wri");
  TYPE (".wrk", "application/x-wrk");
  TYPE (".ws", "application/x-ws");
  TYPE (".ws2", "application/x-ws");
  TYPE (".wsc", "text/scriptlet");
  TYPE (".wsdl", "text/xml");
  TYPE (".wvx", "video/x-ms-wvx");
  TYPE (".xdp", "application/vnd.adobe.xdp");
  TYPE (".xdr", "text/xml");
  TYPE (".xfd", "application/vnd.adobe.xfd");
  TYPE (".xfdf", "application/vnd.adobe.xfdf");
  TYPE (".xhtml", "text/html");
  TYPE (".xls", "application/vnd.ms-excel");
  TYPE (".xls", "application/x-xls");
  TYPE (".xlw", "application/x-xlw");
  TYPE (".xml", "text/xml");
  TYPE (".xpl", "audio/scpls");
  TYPE (".xq", "text/xml");
  TYPE (".xql", "text/xml");
  TYPE (".xquery", "text/xml");
  TYPE (".xsd", "text/xml");
  TYPE (".xsl", "text/xml");
  TYPE (".xslt", "text/xml");
  TYPE (".xwd", "application/x-xwd");
  TYPE (".x_b", "application/x-x_b");
  TYPE (".sis", "application/vnd.symbian.install");
  TYPE (".sisx", "application/vnd.symbian.install");
  TYPE (".x_t", "application/x-x_t");
  TYPE (".ipa", "application/vnd.iphone");
  TYPE (".apk", "application/vnd.android.package-archive");
  TYPE (".xap", "application/x-silverlight-app");
}

bool StaticHandler::IsValid()
{
  const auto &config = Config::GetInstance();
  const auto &list = config.method_list();
  auto method = request_.method();

  return static_cast<bool>(list.count(method));
}

bool StaticHandler::IsExist()
{
  const auto &config = Config::GetInstance();
  auto www_root = config.www_root();
  auto path = www_root + path_;

  Dir dir(path);
  return dir.IsExist(file_);
}

bool StaticHandler::Forbidden()
{
  const auto &config = Config::GetInstance();
  const auto &list = config.forbidden_resource();
  auto www_root = config.www_root();
  auto path = www_root + path_;
  for (const auto &var : list) {
    if (path.find(var) != std::string::npos)
      return true;
  }

  return false;
}