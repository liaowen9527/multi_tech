class LimitCount{
    constructor(limit_day, limit_week, limit_total) {
        limit_day = limit_day || -1;
        limit_week = limit_week || -1;
        limit_total = limit_total || -1;

        this.limit_day = limit_day;
        this.limit_week = limit_week;
        this.limit_total = limit_total;
    }
    tojson(){
        var self = this;
        return {
            limit_day: self.limit_day,
            limit_week: self.limit_week,
            limit_total: self.limit_total
        }
    }
    can_reduce(count) {
        if (this.limit_day > 0 && this.limit_day < count) {
            return false;
        }
        if (this.limit_week > 0 && this.limit_week < count) {
            return false;
        }
        if (this.limit_total > 0 && this.limit_total < count) {
            return false;
        }
        return true;
    }
    reduce(count) {

        if(this.limit_day > 0) {
            this.limit_day = this.limit_day - count;
        }
        if(this.limit_week > 0) {
            this.limit_week = this.limit_week - count;
        }
        if(this.limit_total > 0) {
            this.limit_total = this.limit_total - count;
        }
        return true;
    }

}

module.exports = LimitCount;