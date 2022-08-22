function MaterialType(){
    this.type_p = "P";
    this.desc_p = "psd";

    this.type_z = "Z";
    this.desc_z = "字体";
}

function Material() {
    this._id = "";
    this.num_ = "";
    this.type_ = "";
    this.baidu_shared = "";
    this.taobao_link = "";
    this.desc_ = "";
    this.desc2_ = "";
}

module.exports = Material;
module.exports = MaterialType;